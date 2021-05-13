#include <stdio.h>
#include <stdlib.h>

#include "../headers/instructions.h"


instructions_array_type * make_instructions_array(instruction_type *given_array, size_t length) {
	instructions_array_type *instructions_array = (instructions_array_type *) malloc(sizeof (instructions_array_type) + sizeof (instruction_type) * length);
	if (!instructions_array) {
		puts("Not enough place to allocate an instructions array!");
		exit(1);
	}
	instructions_array->beginning_ptr = given_array;
	instructions_array->current_instruction_ptr = given_array;
	instructions_array->end_ptr = given_array + length - 1;
	return instructions_array;
}


void check_if_there_is_next_instruction(vm_type *vm) {
	if (vm->instructions_array->current_instruction_ptr > vm->instructions_array->end_ptr) {
		puts("No more instructions");
		exit(1);
	}
}


instruction_type get_next_instruction(vm_type *vm) {
	check_if_there_is_next_instruction(vm);
	return *((vm->instructions_array->current_instruction_ptr)++);
}


void vm_goto(vm_type *vm, stack_element_type offset) {
	offset = abs(offset);
	if (vm->instructions_array->beginning_ptr + offset > vm->instructions_array->end_ptr) {
		puts("Out of bounds of an array of instructions");
		exit(1);
	}
	vm->instructions_array->current_instruction_ptr = vm->instructions_array->beginning_ptr + offset;
}
