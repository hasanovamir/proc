#ifndef ASSEMBLER_H
#define ASSEMBLER_H

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

//————————————————————————————————————————————————————————————————————————————————

struct parameters_t
{
    int num_lines;
    int num_el;
    int num_com;
    int num_com_arr_el;
    char*  el_array;
    int*  com_array;
    char** line_array;
};

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    ASM_SUCCESS              = 0,
    ASM_ERR_ALLOCATION       = 1,
    ASM_ERR_OPEN_READ_FILE   = 2,
    ASM_ERR_FREAD            = 3,
    ASM_ERR_LINE_ARR_FILL    = 4,
    ASM_ERR_INVALID_SCAN_SYM = 5,
    ASM_ERR_OPEN_WRITE_FILE  = 6,
}error_t;

//————————————————————————————————————————————————————————————————————————————————

error_t     allocation_el_arr        (parameters_t* data, const char* title);
error_t     allocation_line_arr  (parameters_t* data                   );
error_t     read_commands            (parameters_t* data, const char* title);
error_t     count_n_lines            (parameters_t* data                   );
error_t     filling_line_array       (parameters_t* data                   );
error_t     translator               (parameters_t* data                   );
error_t     write_commands           (parameters_t* data, const char* title);
const char* error_code_to_string     (error_t status                       );
error_t     destroy_arrays           (parameters_t* data                   );
error_t     get_file_size            (parameters_t* data, const char* title);
error_t     allocation_com_arr       (parameters_t* data                   );

//————————————————————————————————————————————————————————————————————————————————

#define RETURN_IF_ERR(status) do \
{\
    error_t _status_val_ = status;\
    if (_status_val_)\
    {\
        fprintf(stderr, "%s in %s:%d func:%s\n", error_code_to_string (_status_val_), __FILE__, __LINE__, __PRETTY_FUNCTION__);\
        return _status_val_;\
    }\
} while (0)

//————————————————————————————————————————————————————————————————————————————————

#ifndef N_DEBUG
#define DEBUG_ASSERT(cond)\
if (!cond)\
{\
    fprintf(stderr, "%s in %s:%d func:%s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    exit(1);\
}
#else
#define DEBUG_ASSERT() 
#endif //N_DEBUG

//————————————————————————————————————————————————————————————————————————————————

#define ERRCASE(enum)\
    case enum:\
        return #enum;\

//————————————————————————————————————————————————————————————————————————————————

#endif //ASSEMBLER_H