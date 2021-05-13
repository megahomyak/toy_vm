#include "instructions_array.h"


typedef int register_type;


typedef struct RegistersFrame {
	instruction_type *return_address;
	struct RegistersFrame *prev_registers_frame;
	size_t last_register_index;
	register_type contents[];
} registers_frame_type;
