typedef int instruction_value_type;


typedef union {
	enum {
		HALT,
		PUSH,
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		OR,
		AND,
		NOT,
		POP,
		DUPLICATE,
		IS_EQUAL,
		IS_GREATER,
		IS_LESS,
		IS_GREATER_OR_EQUAL,
		IS_LESS_OR_EQUAL,
		JUMP,
		JUMP_IF,
		SAVE,
		LOAD,
		CALL,
		RETURN,
		PRINT,
		INPUT,
		MULTIPUSH
	} action;
	instruction_value_type value;
} instruction_type;


typedef struct {
	instruction_type *beginning_ptr;
	instruction_type *current_instruction_ptr;
	instruction_type *end_ptr;
} instructions_array_type;
