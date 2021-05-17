#include "../types/vm.h"


registers_frame_type * make_new_registers_frame(registers_frame_type *prev_registers_frame, size_t registers_amount, instruction_type *return_address);

void remove_registers_frame_from_the_vm(vm_type *vm);

void check_if_index_is_within_the_bounds_of_registers_array(vm_type *vm, size_t register_index);

void save_to_register(vm_type *vm, size_t register_index, register_type value);

register_type load_from_register(vm_type *vm, size_t register_index);
