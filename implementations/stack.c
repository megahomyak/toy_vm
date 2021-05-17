#include <stdio.h>
#include <stdlib.h>

#include "../headers/stack.h"
#include "../cool_stuff/new_macro.h"


stack_frame_type * _make_new_stack_frame(size_t elements_amount, stack_frame_type *prev_stack_frame) {
	stack_frame_type *new_stack_frame = big_new (stack_frame_type, stack_element_type, elements_amount);
	if (!new_stack_frame) {
		puts("Not enough place to allocate a stack frame!");
		exit(1);
	}
	new_stack_frame->prev_stack_frame = prev_stack_frame;
	new_stack_frame->end_ptr = new_stack_frame->contents + elements_amount - 1;
	return new_stack_frame;
}


void add_new_stack_frame_to_the_vm(vm_type *vm) {
	stack_frame_type *stack_frame = _make_new_stack_frame(vm->stack_info.stack_frame_size, vm->stack_info.current_stack_frame);
	vm->stack_info.current_stack_frame = stack_frame;
	vm->stack_info.current_element_ptr = stack_frame->contents - 1;
}


void add_first_stack_frame_to_the_vm(vm_type *vm) {
	stack_frame_type *stack_frame = _make_new_stack_frame(vm->stack_info.stack_frame_size, NULL);
	vm->stack_info.current_stack_frame = stack_frame;
	vm->stack_info.current_element_ptr = stack_frame->contents - 1;
}


void remove_stack_frame_from_the_vm(vm_type *vm) {
	stack_frame_type *old_stack_frame = vm->stack_info.current_stack_frame;
	vm->stack_info.current_stack_frame = vm->stack_info.current_stack_frame->prev_stack_frame;
	free(old_stack_frame);
	vm->stack_info.current_element_ptr = vm->stack_info.current_stack_frame->end_ptr;
}


void check_if_there_is_something_to_pop(vm_type *vm, const char *error_message) {
	if (vm->stack_info.current_element_ptr < vm->stack_info.current_stack_frame->contents) {
		if (vm->stack_info.current_stack_frame->prev_stack_frame) {
			remove_stack_frame_from_the_vm(vm);
		} else {
			puts(error_message);
			exit(1);
		}
	}
}


const char NOTHING_TO_POP[] = "Nothing to pop";


stack_element_type pop(vm_type *vm) {
	check_if_there_is_something_to_pop(vm, NOTHING_TO_POP);
	return pop_without_check(vm);
}


void pop_without_return(vm_type *vm) {
	check_if_there_is_something_to_pop(vm, NOTHING_TO_POP);
	vm->stack_info.current_element_ptr--;
}


stack_element_type peek(vm_type *vm) {
	check_if_there_is_something_to_pop(vm, "Nothing to peek");
	return *(vm->stack_info.current_element_ptr);
}


void push(vm_type *vm, stack_element_type value) {
	if (vm->stack_info.current_element_ptr == vm->stack_info.current_stack_frame->end_ptr) {
		add_new_stack_frame_to_the_vm(vm);
	}
	*(++vm->stack_info.current_element_ptr) = value;
}
