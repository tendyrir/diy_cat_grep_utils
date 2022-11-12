CC = gcc
#ASAN = -fsanitize=address
FLAGS = -Wall -Werror -Wextra -std=c11

all: clean s21_cat

s21_cat:
	$(CC) $(ASAN) $(FLAGS) -o s21_cat s21_cat.c

s21_cat_test: s21_cat
	$(CC) $(ASAN) $(FLAGS) -o s21_cat_test s21_cat_test.c
	sh cat.sh
	./s21_cat_test
	
check:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c
	cppcheck --enable=all --suppress=missingIncludeSystem --force *.c
	
clean:
	rm -f s21_cat s21_cat_test
	rm -f tmp_test_file1 tmp_test_file2
	rm -f test_result1 test_result2
	rm -f log.txt valgrind-out.txt RESULT_VALGRIND.txt CPPLINT.cfg
	rm -rf ./s21_cat_test.dSYM ./s21_cat.dSYM
