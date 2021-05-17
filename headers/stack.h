#include "../types/vm.h"


void add_new_stack_frame_to_the_vm_with_pointer(vm_type *vm, stack_frame_type *stack_frame_to_add_to);

#define add_new_stack_frame_to_the_vm(vm) add_new_stack_frame_to_the_vm_with_pointer(vm, vm->stack_info.current_stack_frame)

#define add_first_stack_frame_to_the_vm(vm) add_new_stack_frame_to_the_vm_with_pointer(vm, NULL)

void remove_stack_frame_from_the_vm(vm_type *vm);

void check_if_there_is_something_to_pop(vm_type *vm, const char *error_message);

#define pop_without_check(vm) *(vm->stack_info.current_element_ptr--)

stack_element_type pop(vm_type *vm);

void pop_without_return(vm_type *vm);

stack_element_type peek(vm_type *vm);

void push(vm_type *vm, stack_element_type value);
