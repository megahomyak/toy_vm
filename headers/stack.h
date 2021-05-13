#include "../types/vm.c"

void add_new_stack_frame_to_the_vm(vm_type *vm);

void add_first_stack_frame_to_the_vm(vm_type *vm);

void remove_stack_frame_from_the_vm(vm_type *vm);

void check_if_there_is_something_to_pop(vm_type *vm, const char *error_message);

#define pop_without_check(vm) *(vm->stack_info.current_element_ptr--)

stack_element_type pop(vm_type *vm);

void pop_without_return(vm_type *vm);

stack_element_type peek(vm_type *vm);

void push(vm_type *vm, stack_element_type value);
