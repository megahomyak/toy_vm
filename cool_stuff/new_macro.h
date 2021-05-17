#ifndef NEW_MACRO_FILE_INCLUDED
#define NEW_MACRO_FILE_INCLUDED


#define new(type) (type *) malloc(sizeof (type))
#define big_new(main_type, contents_type, elements_amount) (main_type *) malloc(sizeof (main_type) + sizeof (contents_type) * (elements_amount))


#endif
