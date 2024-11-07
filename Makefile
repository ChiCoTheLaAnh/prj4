build_main:
	gcc main.c -o app.exe
run_main:
	./app.exe
build_test:
	rm -f test.exe
	gcc -DTEST -o test.exe test.c main.c
run_test: test.exe
	./test.exe
valgrind:
	valgrind --tool=memcheck --leak-check=yes ./app.exe
run_file:
	./app.exe < test.txt
clean:
	rm -f app.exe