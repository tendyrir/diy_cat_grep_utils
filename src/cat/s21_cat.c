/*                                      CAT MANUAL
    |--------------------------------------------------------------------------------------|
    |   -b (--number-nonblank) Number the non-blank output lines, starting at 1.           |
    |                                      ---                                             |
    |   -e (-E) Display non-printing characters (see the -v option),                       |
    |           and display a dollar sign (`$') at the end of each line.                   |
    |                                      ---                                             |
    |   -n (--number) Number the output lines, starting at 1.                              |
    |                                      ---                                             |
    |   -s (--squeeze-blank) Squeeze multiple adjacent empty lines,                        |
    |                        causing the output to be single spaced.                       |
    |                                      ---                                             |
    |   -t (-T) Display non-printing characters (see the -v option),                       |
    |           and display tab characters as `^I'.                                        |
    |                                      ---                                             |
    |   -v Display non-printing characters so they are visible.                            |
    |      Control characters print as `^X' for control-X; the delete                      |
    |      character (octal 0177) prints as `^?'.                                          |
    |      Non-ASCII characters (with the high bit set) are printed as `M-' (for meta)     |
    |      followed by the character for the low 7 bits.                                   |
    |--------------------------------------------------------------------------------------|
*/

#include <stdio.h>     // for printf() function
#include <stdlib.h>    // for exit() in usage
#include <unistd.h>    // for "optind", which is the index of the next element to be processed in argv
#include <getopt.h>    // for "getopt_long()"
#include <ctype.h>     // for isascii and isctrl functions

typedef struct options {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} opts;

void parser(int argc, char *argv[], opts *options);
void reader(char *argv[], opts *options);

int main(int argc, char * argv[]) {
 /* 
    this is for printing: argc, argv, optind
    printf("argc = %d\n", argc);
    for (i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);}
    printf("optind = %d\n", optind);
    int i;
*/
    // - INITIALIZE STRUCT -
    opts options = {0};
    // - PARSER -
    parser(argc, argv, &options);
    // - READER -
    if (options.b) {
        options.n = 0;
    }
    while (optind < argc) {
        reader(argv, &options);
        optind++;
    }
    return 0;
}
void parser(int argc, char *argv[], opts *options) {
    int opt;
    int option_index;
        static struct option long_options[] = {
        {"number-nonblank", 0, 0, 'b'},
        {"number", 0, 0, 'n'},
        {"squeeze-blank", 0, 0, 's'},
        {0, 0, 0, 0},
    };

    while ((opt = getopt_long(argc, argv, "+beEnstTv", long_options, &option_index)) != -1) {
        switch (opt) {
        case 'b':
            options->b = 1;
            break;
        case 'e':
            options->e = 1;
            options->v = 1;
            break;
        case 'n':
            options->n = 1;
            break;
        case 's':
            options->s = 1;
            break;
        case 't':
            options->t = 1;
            options->v = 1;
            break;
        case 'v':
            options->v = 1;
            break;
        case 'T':
            options->t = 1;
            break;
        case 'E':
            options->e = 1;
            break;
        default:
            fprintf(stderr, "usage: cat [-beEnstTv] [file ...]\n");
            exit(1);
        }
    }
}

void reader(char *argv[], opts *options) {
    FILE *f = fopen(argv[optind], "r");
    if (f) {
        int cur;
        int str_count = 0;
        int counter = 0;
        int empty_str_count = 1;
        while ((cur = fgetc(f)) != EOF) {
/*  options 's' */
            if (options->s && cur == '\n') {
                empty_str_count++;
                if (empty_str_count == 3) {
                    empty_str_count--;
                    continue;
                }
            } else {
                    empty_str_count = 0;
            }
/* option 'b' */
            if (options->b) {
                if (cur != '\n') {
                    if (counter == 0) {
                        printf("%6d\t", ++str_count);
                        counter = 1;
                    }
                } else {
                counter = 0;
                }
            }
/*  option 'n' */
            if (options->n) {
                if (counter == 0) {
                    printf("%6d\t", ++str_count);
                    counter = 1;
                }
                if (cur == '\n') {
                    counter = 0;
                }
            }
/*  option 'e' */
            if (cur == '\n') {
                if (options->e) {
                    printf("%c", '$');
                }
/*  option 't' */
            } else if (cur == '\t') {
                if (options->t) {
                    if (putchar('^') == EOF || putchar('I') == EOF)
                        break;
                    continue;
                }
/*  option 'v' */
            } else if (options->v) {
/*                
    --- если нужно обработать от 128 до 159 --- 
                if (!isascii(cur)) {
                    if (cur > 127) {
                        if (putchar('M') == EOF || putchar('-') == EOF)
                            break;
                        cur = toascii(cur);
                     // cur = (int) cut;
                    }
    ==================================================================================================                            
    Функция iscntrl() возвращает ненулевое значение, если ее аргумент ch является управляющим символом, 
    значение которого в ASCII-средах лежит в диапазоне между нулем и 0x1F или равно 0x7F (символ DEL). 
    В противном случае возвращается нуль. 
    ================================================================================================== 
    --- Реализация от Apple ---
    if (iscntrl(cur)) {
        if (putchar('^') == EOF || putchar(cur == '\177' ? '?' : cur | 0100) == EOF) {
        break;
        }
    continue;
    }
*/
                if (cur == 127 || (cur < 32)) {
                    if (cur == 127) {
                        cur -= 64;
                    } else {
                        cur += 64;
                    }
                    printf("^");
                }
            }
        printf("%c", cur);
        }
        fclose(f);
    } else {
        fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[optind]);
        exit(1);
    }
}
