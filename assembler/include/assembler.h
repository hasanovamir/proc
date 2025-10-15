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

struct asm_context_t
{
    buffer_t             source_buffer;
    line_arr_t           parsed_lines;
    bytecode_container_t bytecode_container;
};

//————————————————————————————————————————————————————————————————————————————————

struct fixup_t
{
    int index;
    int label_num;
};

//————————————————————————————————————————————————————————————————————————————————

struct fixup_list_t
{
    fixup_t* fixup_table;
    int   fixup_count;
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
    ERR   =  0,
    PUSH  =  1,
    OUT   =  2,
    ADD   =  3,
    SUB   =  4,
    MUL   =  5,
    DIV   =  6,
    SQRT  =  7,
    HLT   =  8,
    IN    =  9,
    PUSHR = 33,
    POPR  = 42,
    JMP   = 10,
    JE    = 11,
    JNE   = 12,
    JA    = 13,
    JAE   = 14,
    JB    = 15,
    JBE   = 16,
    CALL  = 17,
    RET   = 18,
    FILL_LABELS = 19,
    COMMENT = 20,
} asm_operation_t;

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

struct oper_name_and_idx_t
{
    const char*     name;
    asm_operation_t idx;
};

//————————————————————————————————————————————————————————————————————————————————

registers_t     convert_reg_name_to_num (char* name            );
const char*     error_code_to_string    (error_t status        );
int             get_file_size           (const char* file_name );
asm_operation_t get_oper_idx            (const char* input_oper);
error_t         allocate_line_arr       (asm_context_t* asm_context);
error_t         count_n_lines           (asm_context_t* asm_context);
error_t         fill_line_array         (asm_context_t* asm_context);
void            compile                 (asm_context_t* asm_context);
error_t         destroy_arrays          (asm_context_t* asm_context);
error_t         allocate_com_arr        (asm_context_t* asm_context);
error_t         allocate_el_arr         (asm_context_t* asm_context, const char* file_name);
error_t         read_commands           (asm_context_t* asm_context, const char* file_name);
error_t         write_commands          (asm_context_t* asm_context, const char* file_name);
void            fixup_labels            (asm_context_t* asm_context, fixup_list_t* fixup_list, int* labels);
void            fill_operation_table    (void (** operations_table)(asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels));

//————————————————————————————————————————————————————————————————————————————————

void out_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void add_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void sub_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void mul_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void div_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void sqrt_op      (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void hlt_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void in_op        (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void ret_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void push_op      (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void popr_op      (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void pushr_op     (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void fill_labels  (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void jmp_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void je_op        (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void jne_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void jb_op        (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void jbe_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void ja_op        (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void jae_op       (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void call_op      (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);
void skip_comment (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels);

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