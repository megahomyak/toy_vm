typedef int stack_element_type;


typedef struct StackFrame {
	stack_element_type *end_ptr;
	struct StackFrame *prev_stack_frame;
	stack_element_type contents[];
} stack_frame_type;


typedef struct StackInfo {
	size_t stack_frame_size;
	stack_element_type *current_element_ptr;
	stack_frame_type *current_stack_frame;
} stack_info_type;
