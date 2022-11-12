/*                                      GREP MANUAL
    |--------------------------------------------------------------------------------------|
    |   -e pattern                                                                         |
    |      Specify a pattern used during the search of the input: an input                 |
    |      line is selected if it matches any of the specified patterns.                   |
    |      This option is most useful when multiple -e options are used to                 |
    |      specify multiple patterns, or when a pattern begins with a dash (`-').          |
    |                                      ---                                             |
    |   -i Perform case insensitive matching.  By default, grep is case sensitive.         |
    |                                      ---                                             |
    |   -v Selected lines are those not matching any of the specified patterns.            |
    |                                      ---                                             |
    |   -c Only a count of selected lines is written to standard output.                   |
    |                                      ---                                             |
    |   -l Only the names of files containing selected lines are written to                |
    |      standard output.  grep will only search a file until a match has                |
    |      been found, making searches potentially less expensive.  Path-                  |
    |      names are listed once per file searched.  If the standard input                 |
    |      is searched, the string ``(standard input)'' is written.                        |
    |                                      ---                                             |
    |   -n Each output line is preceded by its relative line number in the                 |
    |      file, starting at line 1.  The line number counter is reset for                 |
    |      each file processed.  This option is ignored if -c, -L, -l, or -q               |
    |      is specified.                                                                   |
    |--------------------------------------------------------------------------------------|
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <regex.h>
#include <string.h>

typedef struct options {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int cnt_file;
    int countFile;
} opts;

void parser(int argc, char *argv[], opts *options, char *pattern);
void reader(char *argv[], int argc, opts *options, char *pattern);
void apply_options(char *argv[], int argc, FILE *f, char *pattern, opts *options);
int run_program(int argc, char *argv[]);
void get_pattern_e(int* counter, char* pattern, char* optarg);

int main(int argc, char *argv[]) {
    run_program(argc, argv);
    return 0;
}

int run_program(int argc, char *argv[]) {
    char pattern[1024] = {0};
    opts options = {0};
    // запуск парсера
    parser(argc, argv, &options, pattern);
    // отключает флаги для л
    if (options.l || options.c) {
        options.i = 0;
        options.n = 0;
        options.v = 0;
    }

    // при отсутствие ешки
    if (!options.e) {
        strcat(pattern, argv[optind]);
        optind++;
    }
    // запуск ридера
    while (optind < argc) {
        reader(argv, argc, &options, pattern);
        optind++;
    }
    return 0;
}

void parser(int argc, char *argv[], opts *options, char * pattern) {
    int counter = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "e:inclv", NULL, NULL)) != -1) {
        switch (opt) {
        case 'e':
            get_pattern_e(&counter, pattern, optarg);
            options->e = 1;
            break;
        case 'i':
            options->i = 1;
            break;
        case 'v':
            options->v = 1;
            break;
        case 'c':
            options->c = 1;
            break;
        case 'l':
            options->l = 1;
            break;
        case 'n':
            options->n = 1;
            break;
        default:
            fprintf(stderr, "usage: grep [e:ivcln] [file ...]\n");
            exit(1);
        }
    }
}

void get_pattern_e(int* counter, char* pattern, char* optarg) {
    if (*counter > 0)  {
        strcat(pattern, "|");
    }
    *counter = *counter + 1;
    strcat(pattern, optarg);
}

void reader(char *argv[], int argc, opts *options, char *pattern) {
    FILE *f = fopen(argv[optind], "r");
    if (f) {
        apply_options(argv, argc, f, pattern, options);
        fclose(f);
    } else {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[optind]);
    }
}

void apply_options(char *argv[], int argc, FILE *f, char *pattern, opts *options) {
    regex_t reg[2048];
    char *str = NULL;
    size_t len = 0;
    int str_count = 1;
    int counter = 0;
    int match = 0;
/* option -i */
    if (options->i) {
        regcomp(reg, pattern, 0003);  // REG_ICASE 0002 + REG_EXTENDED 0001
    } else {
        regcomp(reg, pattern, 0001);  // REG_EXTENDED 0001
    }
    while ((getline(&str, &len, f) != -1)) {
        if (optind != argc - 1) {
            options->cnt_file = 1;
        }
        int err = 0;
        err = regexec(reg, str, 0, NULL, 0);
/* option -v */
        if (err == 1) {
            if (options->v) {
                if (options->cnt_file) {
                    printf("%s:", argv[optind]);
                }
                printf("%s", str);
            }
        }
/* option -n */
        if (err == 0 && !options->v && !options->l && !options->c) {
            if (options->cnt_file > 1) {
                printf("%s:", argv[optind]);
            }
            if (options->n) {
                    printf("%d:", str_count);
                }
            printf("%s", str);
        }
/* option -l */
        if (err == 0 && counter == 0) {
            counter++;
            if (options->l) {
                printf("%s\n", argv[optind]);
            }
        }
        if (err == 0) {
            match++;
        }
        str_count++;
    }
/* option -c */
    if (options->c) {
        if (options->cnt_file) {
            printf("%s:", argv[optind]);
        }
        printf("%d\n", match);
    }
    regfree(reg);
    free(str);
}
