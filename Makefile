all:
	gcc main.c implementations/stack.c implementations/instructions.c implementations/registers.c -o main.exe -Wall -Wextra -std=c11

run:
	./main.exe
