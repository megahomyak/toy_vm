#include <stdlib.h>
#include <stdio.h>

#include "../headers/registers.h"
#include "../cool_stuff/new_macro.h"


registers_frame_type * make_new_registers_frame(registers_frame_type *prev_registers_frame, size_t registers_amount, instruction_type *return_address) {
	registers_frame_type *new_registers_frame = big_new (registers_frame_type, register_type, registers_amount);
	if (!new_registers_frame) {
		puts("Not enough place to allocate a registers frame!");
		exit(1);
	}
	new_registers_frame->return_address = return_address;
	new_registers_frame->prev_registers_frame = prev_registers_frame;
	new_registers_frame->last_register_index = registers_amount - 1;
	return new_registers_frame;
}


void remove_registers_frame_from_the_vm(vm_type *vm) {
	registers_frame_type *top_frame = vm->current_registers_frame;
	vm->instructions_array->current_instruction_ptr = top_frame->return_address;
	vm->current_registers_frame = top_frame->prev_registers_frame;
	free(top_frame);
}


void check_if_index_is_within_the_bounds_of_registers_array(vm_type *vm, size_t register_index) {
	if (vm->current_registers_frame->last_register_index < register_index) {
		puts("Out of bounds of an array of registers");
		exit(1);
	}
}


void save_to_register(vm_type *vm, size_t register_index, register_type value) {
	check_if_index_is_within_the_bounds_of_registers_array(vm, register_index);
	vm->current_registers_frame->contents[register_index] = value;
}


register_type load_from_register(vm_type *vm, size_t register_index) {
	check_if_index_is_within_the_bounds_of_registers_array(vm, register_index);
	return vm->current_registers_frame->contents[register_index];
}
