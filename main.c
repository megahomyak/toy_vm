#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers/registers.h"
#include "headers/instructions.h"
#include "headers/stack.h"


#define VM_DEBUG


#ifdef VM_DEBUG
	#define vm_case(s) case s: puts(#s);
#else
	#define vm_case(s) case s: ;
#endif


vm_type * make_vm(size_t stack_frame_size, size_t first_registers_frame_size, instructions_array_type *instructions_array) {
	vm_type *vm = (vm_type *) malloc(sizeof (vm_type));
	if (!vm) {
		puts("Not enough place to allocate a vm!");
		exit(1);
	}
	vm->current_registers_frame = make_new_registers_frame(
		NULL,  // Previous registers frame
		first_registers_frame_size,
		NULL  // Return address
	);
	vm->is_halted = 0;
	vm->instructions_array = instructions_array;
	vm->stack_info.stack_frame_size = stack_frame_size;
	add_first_stack_frame_to_the_vm(vm);
	return vm;
}


void free_vm(vm_type *vm) {
	free(vm->stack_info.current_stack_frame);
	free(vm->current_registers_frame);
	free(vm->instructions_array);
	free(vm);
}


void process_next_instruction(vm_type *vm) {
	/*
	Only PUSH and MULTIPUSH require additional information after them:

	PUSH [VALUE]
	MULTIPUSH [AMOUNT OF VALUES] [VALUE 1] [VALUE 2] [VALUE 3] [...]


	Other instructions may require additional information from stack:

	Format:
		INSTRUCTION_NAME:
			[VALUE ON TOP OF THE STACK] [LOWER VALUE] [ARGUMENTS ARE OPTIONAL]
			-> [VALUE THAT'S BEING PUSHED (OPTIONAL)]

	HALT:
	ADD:
		[FIRST OPERAND] [SECOND OPERAND] (order doesn't matter)
		-> [RESULT]
	SUBTRACT:
		[SECOND OPERAND] [FIRST OPERAND]
		-> [RESULT]
	MULTIPLY:
		[FIRST OPERAND] [SECOND OPERAND] (order doesn't matter)
		-> [RESULT]
	DIVIDE:
		[SECOND OPERAND] [FIRST OPERAND]
		-> [RESULT]
	OR:
		[FIRST OPERAND] [SECOND OPERAND] (order doesn't matter)
		-> [BOOL]
	AND:
		[FIRST OPERAND] [SECOND OPERAND] (order doesn't matter)
		-> [BOOL]
	NOT:
		[VALUE]
		-> [BOOL]
	POP:
		[VALUE]
	DUPLICATE:
		[VALUE]
	IS_EQUAL:
		[FIRST OPERAND] [SECOND OPERAND] (order doesn't matter)
		-> [BOOL]
	IS_GREATER:
		[SECOND OPERAND] [FIRST OPERAND]
		-> [BOOL]
	IS_LESS:
		[SECOND OPERAND] [FIRST OPERAND]
		-> [BOOL]
	IS_GREATER_OR_EQUAL:
		[SECOND OPERAND] [FIRST OPERAND]
		-> [BOOL]
	IS_LESS_OR_EQUAL:
		[SECOND OPERAND] [FIRST OPERAND]
		-> [BOOL]
	JUMP:
		[DESTINATION INSTRUCTION INDEX]
	JUMP_IF:
		[CONDITION] [DESTINATION INSTRUCTION INDEX]
	SAVE:
		[REGISTER INDEX] [VALUE]
	LOAD:
		[REGISTER INDEX]
		-> [VALUE FROM REGISTER]
	CALL:
		[REGISTERS AMOUNT] [DESTINATION INSTRUCTION INDEX]
	RETURN:
	PRINT:
		[CHARACTER CODE]

	Also you can look at the implementations below to fully understand how it works.
	*/
	instruction_type instruction = get_next_instruction(vm);
	stack_element_type right_operand;  // Needed to do some operations
	switch (instruction.action) {
		vm_case (HALT)
			vm->is_halted = 1;
			if (vm->current_registers_frame->prev_registers_frame) {
				puts("Removing register frames...");
			}
			for (registers_frame_type *registers_frame = vm->current_registers_frame; registers_frame->prev_registers_frame; registers_frame = registers_frame->prev_registers_frame) {
				remove_registers_frame_from_the_vm(vm);
			}
			fputs("Done. Unwinding the stack (from top to bottom):", stdout);
			if (vm->stack_info.current_stack_frame->contents > vm->stack_info.current_element_ptr && !vm->stack_info.current_stack_frame->prev_stack_frame) {
				puts(" *empty*");
			} else {
				for (;;) {
					if (vm->stack_info.current_element_ptr < vm->stack_info.current_stack_frame->contents) {
						if (vm->stack_info.current_stack_frame->prev_stack_frame) {
							remove_stack_frame_from_the_vm(vm);
						} else {
							break;
						}
					}
					printf(" %d", pop_without_check(vm));
				}
				putchar('\n');
			}
			return;
		vm_case (PUSH)
			push(vm, get_next_instruction(vm).value);
			break;
		vm_case (ADD)
			push(vm, pop(vm) + pop(vm));
			break;
		vm_case (SUBTRACT)
			right_operand = pop(vm);
			push(vm, pop(vm) - right_operand);
			break;
		vm_case (MULTIPLY)
			push(vm, pop(vm) * pop(vm));
			break;
		vm_case (DIVIDE)
			right_operand = pop(vm);
			push(vm, pop(vm) / right_operand);
			break;
		vm_case (OR)
			push(vm, pop(vm) || pop(vm));
			break;
		vm_case (AND)
			push(vm, pop(vm) && pop(vm));
			break;
		vm_case (NOT)
			push(vm, !pop(vm));
			break;
		vm_case (POP)
			pop_without_return(vm);
			break;
		vm_case (DUPLICATE)
			push(vm, peek(vm));
			break;
		vm_case (IS_EQUAL)
			push(vm, pop(vm) == pop(vm));
			break;
		vm_case (IS_GREATER)
			right_operand = pop(vm);
			push(vm, pop(vm) > right_operand);
			break;
		vm_case (IS_LESS)
			right_operand = pop(vm);
			push(vm, pop(vm) < right_operand);
			break;
		vm_case (IS_GREATER_OR_EQUAL)
			right_operand = pop(vm);
			push(vm, pop(vm) >= right_operand);
			break;
		vm_case (IS_LESS_OR_EQUAL)
			right_operand = pop(vm);
			push(vm, pop(vm) <= right_operand);
			break;
		vm_case (JUMP)
			vm_goto(vm, pop(vm));
			break;
		vm_case (JUMP_IF)
			if (pop(vm)) {
				vm_goto(vm, pop(vm));
			} else {
				pop_without_return(vm);
			}
			break;
		vm_case (SAVE)
			save_to_register(vm, pop(vm), pop(vm));
			break;
		vm_case (LOAD)
			push(vm, load_from_register(vm, pop(vm)));
			break;
		vm_case (CALL)
			vm->current_registers_frame = make_new_registers_frame(
				vm->current_registers_frame,  // Previous registers frame
				pop(vm),  // Registers amount
				vm->instructions_array->current_instruction_ptr  // Return address
			);
			vm_goto(vm, pop(vm));
			break;
		vm_case (RETURN)
			if (vm->current_registers_frame->return_address) {
				remove_registers_frame_from_the_vm(vm);
			} else {
				puts("Nowhere to return!");
				exit(1);
			}
			break;
		vm_case (PRINT)
			putchar(pop(vm));
			break;
		vm_case (INPUT)
			push(vm, getchar());
			break;
		vm_case (MULTIPUSH)
			for (instruction_value_type values_amount = get_next_instruction(vm).value; values_amount != 0; --values_amount) {
				push(vm, get_next_instruction(vm).value);
			}
			break;
		default:
			printf("Unknown opcode: %d", instruction.value);
			exit(1);
	}
}


void run_vm(vm_type *vm) {
	while (!vm->is_halted) {
		process_next_instruction(vm);
	}
}


#define STACK_FRAME_SIZE 16
#define FIRST_REGISTERS_FRAME_SIZE 16


int main(void) {
	instruction_type instructions[] = {
		{MULTIPUSH}, {2}, {20}, {10}, {HALT}
	};
	vm_type *vm = make_vm(
		STACK_FRAME_SIZE,
		FIRST_REGISTERS_FRAME_SIZE,
		make_instructions_array(
			instructions,
			sizeof (instructions) / sizeof (instruction_type)
		)
	);
	run_vm(vm);
}
