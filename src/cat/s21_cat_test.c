#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int len = rand() % 10000;
    FILE *fd1 = fopen("tmp_test_file1", "w");
    FILE *fd2 = fopen("tmp_test_file2", "w");
        if (fd1) {
            for (int i = 0; i < len; i++) {
                unsigned char c = rand() % 127;
                fputc(c, fd1);
                fputc(c, fd2);
            }
        fclose(fd1);
        fclose(fd2);
        } else {
            perror("");
        }
        char *options[] = {"-b", "-e", "-n", "-s", "-t", "-v"};
        int counter_passed = 0;
        int counter_failed = 0;
        int total_count = 0;
        int prc = 0;
        for (int i = 0; i < 6; i++) {
            char command1[512] = {'\0'};
            char command2[512] = {'\0'};
            strcat(command1, "cat ");
            strcat(command2, "./s21_cat ");
            strcat(command1, options[i]);
            strcat(command2, options[i]);
            // cat [OPTION] ./tmp_test_file > test_result1
            strcat(command1, " tmp_test_file1 > test_result1");
            // ./s21cat [OPTION] ./tmp_test_file > test_result2
            strcat(command2, " tmp_test_file2 > test_result2");
            system(command1);
            system(command2);
            printf("%s\n", command1);
            printf("%s\n", command2);
            if (system("diff test_result1 test_result2")) {
                printf("\e[1;31mFAIL\033[0;37m\n");
                counter_failed++;
            } else {
                printf("\033[1;32mPASSED!\033[0;37m\n");
                counter_passed++;
            }
            total_count++;
        }
    prc = 100 / (total_count / counter_passed);
    printf("TOTAL: %d%% \n%d CHECKS \n%d \033[1;32mPASSED\033[0;37m \n%d \e[1;31mFAILED\033[0;37m\n",
            prc, total_count, counter_passed, counter_failed);
    return 0;
}
