#include "../types/vm.h"


instructions_array_type * make_instructions_array(instruction_type *given_array, size_t length);

void check_if_there_is_next_instruction(vm_type *vm);

instruction_type get_next_instruction(vm_type *vm);

void vm_goto(vm_type *vm, stack_element_type offset);
