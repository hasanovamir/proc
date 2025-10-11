#ifndef ASSEMBLER_H
#define ASSEMBLER_H

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

//————————————————————————————————————————————————————————————————————————————————

struct line_arr_t
{
    int    num_lines;
    char** line_array;
};

//————————————————————————————————————————————————————————————————————————————————

struct buffer_t
{
    int   buffer_size;
    char* source_code_array;
};

//————————————————————————————————————————————————————————————————————————————————

struct bytecode_container_t
{
    int  num_commands;
    int  num_bytecode_elements;
    int* bytecode;
};

//————————————————————————————————————————————————————————————————————————————————

struct context_t
{
    buffer_t             source_buffer;
    line_arr_t           parsed_lines;
    bytecode_container_t bytecode_container;
};

//————————————————————————————————————————————————————————————————————————————————

// struct label_t
// {
//     size_t index;
//     size_t number;
// };

//————————————————————————————————————————————————————————————————————————————————

struct fixup_t
{
    size_t index;
    size_t label_num;
};

//————————————————————————————————————————————————————————————————————————————————

struct fixup_list_t
{
    fixup_t* fixup_table;
    size_t   fix_up_count;
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
    ASM_ERR_UNKNOWN_REG_NAME = 7,
} error_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    ERROR = -1,
    REG1  =  1,
    REG2  =  2,
    REG3  =  3,
    REG4  =  4,
    REG5  =  5,
    REG6  =  6,
    REG7  =  7,
    REG8  =  8,
    REG9  =  9,
    REG10 = 10,
    REG11 = 11,
    REG12 = 12,
    REG13 = 13,
    REG14 = 14,
    REG15 = 15,
    REG16 = 16,
} registers_t;

//————————————————————————————————————————————————————————————————————————————————

error_t     allocate_el_arr          (context_t* context, const char* file_name);
error_t     allocate_line_arr        (context_t* context                   );
error_t     read_commands              (context_t* context, const char* file_name);
error_t     count_n_lines              (context_t* context                   );
error_t     fill_line_array            (context_t* context                   );
error_t     compile                    (context_t* context, int* labels      );
error_t     write_commands             (context_t* context, const char* file_name);
const char* error_code_to_string       (error_t status                       );
error_t     destroy_arrays             (context_t* context                   );
int         get_file_size              (const char* file_name                );
error_t     allocate_com_arr           (context_t* context                   );
registers_t convert_name_of_reg_to_num (char* name                           );

//————————————————————————————————————————————————————————————————————————————————

void push_op  (context_t* context, int* pos, int i);
void out_op   (context_t* context, int* pos);
void add_op   (context_t* context, int* pos);
void sub_op   (context_t* context, int* pos);
void mul_op   (context_t* context, int* pos);
void div_op   (context_t* context, int* pos);
void sqrt_op  (context_t* context, int* pos);
void hlt_op   (context_t* context, int* pos);
void in_op    (context_t* context, int* pos);
void popr_op  (context_t* context, int* pos, int i);
void pushr_op (context_t* context, int* pos, int i);
void jmp_op   (context_t* context, int* pos, int i);

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