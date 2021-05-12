#ifndef VM_TYPE_FILE_INCLUDED
#define VM_TYPE_FILE_INCLUDED

#include "registers_frame.c"
#include "stack_frame.c"


typedef struct {
	unsigned char is_halted;
	stack_info_type stack_info;
	registers_frame_type *current_registers_frame;
	instructions_array_type *instructions_array;
} vm_type;


#endif
