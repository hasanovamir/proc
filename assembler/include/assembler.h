#ifndef ASSEMBLER_H
#define ASSEMBLER_H

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdbool.h>

//————————————————————————————————————————————————————————————————————————————————

typedef int spu_data_t;

const int fixup_array_size = 20;

const int labels_size = 20;

//————————————————————————————————————————————————————————————————————————————————

struct line_arr_t
{
    long long num_lines;
    char**    line_array;
};

//————————————————————————————————————————————————————————————————————————————————

struct buffer_t
{
    long long buffer_size;
    char*     source_code_array;
};

//————————————————————————————————————————————————————————————————————————————————

struct bytecode_container_t
{
    int  num_commands;
    int  num_bytecode_elements;
    int* bytecode;
};

//————————————————————————————————————————————————————————————————————————————————

struct fixup_t
{
    int index;
    int label_num;
};

//————————————————————————————————————————————————————————————————————————————————

struct fixup_context_t
{
    fixup_t* fixup_table;
    int      fixup_count;
    int      fixup_capacity;
};

//————————————————————————————————————————————————————————————————————————————————

struct asm_context_t
{
    buffer_t             source_buffer;
    line_arr_t           parsed_lines;
    bytecode_container_t bytecode_container;
    fixup_context_t      fixup_context;
    int                  labels[labels_size];
    const char*          read_file_name;
    const char*          write_file_name;
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
    ASM_INCORRECT_FILE_NAME  = 8,
} asm_error_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    ERR   = -1,
    PUSH  =  0,
    OUT   =  1,
    ADD   =  2,
    SUB   =  3,
    MUL   =  4,
    DIV   =  5,
    SQRT  =  6,
    HLT   =  7,
    IN    =  8,
    JMP   =  9,
    JE    = 10,
    JNE   = 11,
    JA    = 12,
    JAE   = 13,
    JB    = 14,
    JBE   = 15,
    CALL  = 16,
    RET   = 17,
    PUSHR = 18,
    POPR  = 19,
    PUSHM = 20,
    POPM  = 21,
    DRAW  = 22,
    MEOW  = 23,
    FILL_LABELS,
    FUNC_COUNT,
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

const oper_name_and_idx_t oper_name_and_idx[] = 
{
    {"PUSH" , PUSH },
    {"OUT"  , OUT  },
    {"ADD"  , ADD  },
    {"SUB"  , SUB  },
    {"MUL"  , MUL  },
    {"DIV"  , DIV  },
    {"SQRT" , SQRT },
    {"HLT"  , HLT  },
    {"IN"   , IN   },
    {"JMP"  , JMP  },
    {"JE"   , JE   },
    {"JNE"  , JNE  },
    {"JA"   , JA   },
    {"JAE"  , JAE  },
    {"JB"   , JB   },
    {"JBE"  , JBE  },
    {"CALL" , CALL },
    {"RET"  , RET  },
    {"PUSHR", PUSHR},
    {"POPR" , POPR },
    {"PUSHM", PUSHM},
    {"POPM" , POPM },
    {"DRAW" , DRAW },
    {"MEOW" , MEOW }
};

//————————————————————————————————————————————————————————————————————————————————

const char*     error_code_to_string         (asm_error_t status        );
asm_operation_t get_oper_idx                 (const char* input_oper    );
asm_error_t     allocate_line_arr            (asm_context_t* asm_context);
asm_error_t     count_n_lines                (asm_context_t* asm_context);
asm_error_t     fill_line_array              (asm_context_t* asm_context);
void            assemble                     (asm_context_t* asm_context);
asm_error_t     asm_destroy                  (asm_context_t* asm_context);
asm_error_t     allocate_bytecode_array      (asm_context_t* asm_context);
asm_error_t     read_source_code             (asm_context_t* asm_context);
asm_error_t     write_bytecode               (asm_context_t* asm_context);
void            fixup_labels                 (asm_context_t* asm_context);
void            upsize_fixup_context_if_need (asm_context_t* asm_context);
int             check_is_line_empty          (asm_context_t* asm_context, int i);
asm_error_t     allocate_el_arr              (asm_context_t* asm_context, int argc, char** argv);

//————————————————————————————————————————————————————————————————————————————————

void push_op            (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation);
void fill_labels        (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation);
void ram_op             (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation);
void no_arg_op          (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation);
void reg_arg_op         (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation);
void jmp_core_op        (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation);
void fill_fixup_context (asm_context_t* asm_context, int* pos, int input_label_num    );

//————————————————————————————————————————————————————————————————————————————————

void (* const operations_table[FUNC_COUNT])(asm_context_t* asm_context, int* pos, int i, asm_operation_t operation) = 
    {   [PUSH]        = &push_op,
        [OUT]         = &no_arg_op,
        [ADD]         = &no_arg_op,
        [SUB]         = &no_arg_op,
        [MUL]         = &no_arg_op,
        [DIV]         = &no_arg_op,
        [SQRT]        = &no_arg_op,
        [HLT]         = &no_arg_op,
        [IN]          = &no_arg_op,
        [JMP]         = &jmp_core_op,
        [JE]          = &jmp_core_op,
        [JNE]         = &jmp_core_op,
        [JA]          = &jmp_core_op,
        [JAE]         = &jmp_core_op,
        [JB]          = &jmp_core_op,
        [JBE]         = &jmp_core_op,
        [CALL]        = &jmp_core_op,
        [RET]         = &no_arg_op,
        [PUSHR]       = &reg_arg_op,
        [POPR]        = &reg_arg_op,
        [PUSHM]       = &ram_op,
        [POPM]        = &ram_op,
        [DRAW]        = &no_arg_op,
        [MEOW]        = &no_arg_op,
        [FILL_LABELS] = &fill_labels   };

//————————————————————————————————————————————————————————————————————————————————
#define N_DEBUG

#ifndef N_DEBUG
#define DEBUG_ASSERT(cond)\
if (!cond)\
{\
    fprintf(stderr, "%s in %s:%d func:%s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    exit(1);\
}
#else
#define DEBUG_ASSERT(cond) ;
#endif //N_DEBUG

//————————————————————————————————————————————————————————————————————————————————

#define ERRCASE(enum)\
    case enum:\
        return #enum;\

//————————————————————————————————————————————————————————————————————————————————

#define PRINTERR(str)\
fprintf (stderr, "%s in %s:%d in %s", #str, __FILE__, __LINE__, __PRETTY_FUNCTION__);

//————————————————————————————————————————————————————————————————————————————————

#endif //ASSEMBLER_H