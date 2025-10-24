#ifndef SPU_h
#define SPU_h

//————————————————————————————————————————————————————————————————————————————————

#include <unistd.h>

//————————————————————————————————————————————————————————————————————————————————

#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RESET   "\033[0m"

//————————————————————————————————————————————————————————————————————————————————

const int fps            = 30;
const int frame_width    = 96;
const int frame_height   = 36;
const int frame_duration = 1000000 / fps;

//————————————————————————————————————————————————————————————————————————————————

typedef   stack_val_t spu_data_t;

const int spu_stack_size = 100;
const int spu_ret_labels_stack_size = 20;
const int reg_capacity = 16;
const int ram_size = frame_height * (frame_width + 1);
const int sqrt_ram_size = (int) sqrt (ram_size);

//————————————————————————————————————————————————————————————————————————————————

struct spu_context_t
{
    long long   bytecode_size;
    long long   num_completed_commands;
    spu_data_t* bytecode;
    stack_t     stk;
    stack_t     return_labels;
    const char* file_name;
    int         reg[reg_capacity];
    spu_data_t* ram;
};

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
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
    DBA   = 24,
    FUNC_COUNT,
} spu_operation_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    SPU_SUCCESS             = 0,
    SPU_ALLOCATION_ERR      = 1,
    SPU_OPEN_FILE_ERR       = 2,
    SPU_FREAD_ERR           = 3,
    SPU_LACK_OF_HALT        = 4,
    SPU_SCAN_ERR            = 5,
    SPU_NO_EL_TO_POP        = 6,
    SPU_LACK_OF_FILE        = 7,
    SPU_INCORRECT_FILE_SIZE = 8,
} spu_err_t;

//————————————————————————————————————————————————————————————————————————————————

const char* error_spu_code_to_string  (spu_err_t status          );
spu_err_t   allocate_bytecode_array   (spu_context_t* spu_context);
spu_err_t   read_bytecode             (spu_context_t* spu_context);
spu_err_t   spu_execute_instructions  (spu_context_t* spu_context);
spu_err_t   spu_destroy               (spu_context_t* spu_context);
spu_err_t   spu_init                  (spu_context_t* spu_context, int argc, char** argv);

//————————————————————————————————————————————————————————————————————————————————

spu_err_t spu_out   (spu_context_t* spu_context, int* pos);
spu_err_t spu_in    (spu_context_t* spu_context, int* pos);
spu_err_t spu_add   (spu_context_t* spu_context, int* pos);
spu_err_t spu_sub   (spu_context_t* spu_context, int* pos);
spu_err_t spu_div   (spu_context_t* spu_context, int* pos);
spu_err_t spu_mul   (spu_context_t* spu_context, int* pos);
spu_err_t spu_sqrt  (spu_context_t* spu_context, int* pos);
spu_err_t spu_push  (spu_context_t* spu_context, int* pos);
spu_err_t spu_jmp   (spu_context_t* spu_context, int* pos);
spu_err_t spu_je    (spu_context_t* spu_context, int* pos);
spu_err_t spu_jne   (spu_context_t* spu_context, int* pos);
spu_err_t spu_ja    (spu_context_t* spu_context, int* pos);
spu_err_t spu_jae   (spu_context_t* spu_context, int* pos);
spu_err_t spu_jb    (spu_context_t* spu_context, int* pos);
spu_err_t spu_jbe   (spu_context_t* spu_context, int* pos);
spu_err_t spu_pushr (spu_context_t* spu_context, int* pos);
spu_err_t spu_popr  (spu_context_t* spu_context, int* pos);
spu_err_t spu_call  (spu_context_t* spu_context, int* pos);
spu_err_t spu_ret   (spu_context_t* spu_context, int* pos);
spu_err_t spu_pushm (spu_context_t* spu_context, int* pos);
spu_err_t spu_popm  (spu_context_t* spu_context, int* pos);
spu_err_t spu_draw  (spu_context_t* spu_context, int* pos);
spu_err_t spu_meow  (spu_context_t* spu_context, int* pos);
spu_err_t spu_dba   (spu_context_t* spu_context, int* pos);

//————————————————————————————————————————————————————————————————————————————————

spu_err_t (* const commands_table[FUNC_COUNT])(spu_context_t* spu_context_t, int* pos) = 
{   
    [PUSH]    = &spu_push,
    [OUT]     = &spu_out,
    [ADD]     = &spu_add,
    [SUB]     = &spu_sub,
    [MUL]     = &spu_mul,
    [DIV]     = &spu_div,
    [SQRT]    = &spu_sqrt,
    [HLT]     = NULL,
    [IN]      = &spu_in,
    [JMP]     = &spu_jmp,
    [JE]      = &spu_je,
    [JNE]     = &spu_jne,
    [JA]      = &spu_ja,
    [JAE]     = &spu_jae,
    [JB]      = &spu_jb,
    [JBE]     = &spu_jbe,
    [CALL]    = &spu_call,
    [RET]     = &spu_ret,
    [PUSHR]   = &spu_pushr,
    [POPR]    = &spu_popr,
    [PUSHM]   = &spu_pushm,
    [POPM]    = &spu_popm,
    [DRAW]    = &spu_draw,
    [MEOW]    = &spu_meow,
    [DBA]     = &spu_dba,   
};

//————————————————————————————————————————————————————————————————————————————————

#define PRINTERR(str)\
fprintf (stderr, "%s in %s:%d in %s", #str, __FILE__, __LINE__, __PRETTY_FUNCTION__);

//————————————————————————————————————————————————————————————————————————————————

#define POP_TWICE(pointer_1, pointer_2, stk)\
if (stack_pop (stk, pointer_1) == STACK_ALLOCATION_ERR)\
    {\
        PRINTERR (SPU_ALLOCATION_ERR);\
        return SPU_ALLOCATION_ERR;\
    }\
    if (stack_pop (stk, pointer_2) == STACK_ALLOCATION_ERR)\
    {\
        PRINTERR (SPU_ALLOCATION_ERR);\
        return SPU_ALLOCATION_ERR;\
    }

//————————————————————————————————————————————————————————————————————————————————

#define PUSH_TWICE(pointer_1, pointer_2, stk)\
if (stack_pop (stk, pointer_1) == STACK_ALLOCATION_ERR)\
    {\
        PRINTERR (SPU_ALLOCATION_ERR);\
        return SPU_ALLOCATION_ERR;\
    }\
    if (stack_pop (stk, pointer_2) == STACK_ALLOCATION_ERR)\
    {\
        PRINTERR (SPU_ALLOCATION_ERR);\
        return SPU_ALLOCATION_ERR;\
    }

//————————————————————————————————————————————————————————————————————————————————

#endif //SPU.H