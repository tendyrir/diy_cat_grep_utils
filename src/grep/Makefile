CC = gcc
# ASAN = -fsanitize=address
FLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: clean s21_grep

s21_grep:
	$(CC) $(ASAN) $(FLAGS) -o s21_grep s21_grep.c

s21_grep_test: s21_grep
# $(CC) $(ASAN) $(FLAGS) -o s21_grep_test s21_grep_test.c
	sh grep.sh

	
check:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c
	cppcheck --enable=all --suppress=missingIncludeSystem --force *.c
	
clean:
	rm -f s21_grep s21_grep_test
	rm -f tmp_test_file1 tmp_test_file2
	rm -f test_result1 test_result2
	rm -f log.txt valgrind-out.txt RESULT_VALGRIND.txt CPPLINT.cfg
	rm -rf ./s21_grep_test.dSYM ./s21_grep.dSYM
