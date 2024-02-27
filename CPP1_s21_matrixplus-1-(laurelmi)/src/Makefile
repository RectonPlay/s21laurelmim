CC = g++
FLAGS = -std=c++17 -Wall -Wextra -Werror
FUNC = s21_struct.cc s21_functions.cc s21_operators.cc
FUNC_O = s21_struct.o s21_functions.o s21_operators.o
TEST = tests.cc

all: test

clean:
	rm -rf *.o report *.info *.gcda *.gcno *.gcov *.gch *.out *.a *.txt test main

test: clean s21_matrix_oop.a
	$(CC) $(FLAGS) -fprofile-arcs -ftest-coverage $(TEST) s21_matrix_oop.a -lgtest -o test
	./test

s21_matrix_oop.a:
	gcc $(FLAGS) -c $(FUNC)
	ar rc s21_matrix_oop.a $(FUNC_O)
	ranlib s21_matrix_oop.a 

gcov_report: clean
	$(CC) $(FLAGS) -fprofile-arcs -ftest-coverage $(FUNC) $(TEST) -lgtest -o test
	./test
	gcov *.gcda
	gcovr 
	mkdir s21_test_report1
	gcovr --html-details s21_test_report1/index.html
	rm -f *.gcov *.gcda *.gcno test
	open s21_test_report1/index.html

style:
	cp ../materials/linters/.clang-format ./
#	clang-format -i *.cc
	clang-format -n *.cc
 
