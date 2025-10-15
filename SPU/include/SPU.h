#ifndef SPU_h
#define SPU_h

//————————————————————————————————————————————————————————————————————————————————

struct spu_context_t
{
    int  command_array_size;
    int  num_completed_commands;
    int* command_array;
};

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
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
} spu_operation_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    SPU_SUCCESS        = 0,
    SPU_ALLOCATION_ERR = 1,
    SPU_OPEN_FILE_ERR  = 2,
    SPU_FREAD_ERR      = 3,
    SPU_LACK_OF_HALT   = 4,
    SPU_SCAN_ERR       = 5,
    SPU_NO_EL_TO_POP   = 6,
    SPU_HLT            = 7,
} spu_err_t;

//————————————————————————————————————————————————————————————————————————————————

spu_err_t   allocation_command_array  (spu_context_t* spu_context, const char* title     );
spu_err_t   read_commands             (spu_context_t* spu_context, const char* title     );
spu_err_t   spu_execute_instructions  (spu_context_t* spu_context, stack_t* stk,int* reg );
const char* error_spu_code_to_string  (spu_err_t status                                  );
spu_err_t   spu_destroy_command_array (spu_context_t* spu_context, stack_t* stk          );
int         get_file_size             (const char* file_name                             );
spu_err_t   spu_init                  (spu_context_t* spu_context, const char* title, stack_t* stk);
void        fill_command_table        (spu_err_t (** commands_table)(stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels));

//————————————————————————————————————————————————————————————————————————————————

spu_err_t   spu_pop   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_out   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_in    (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_add   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_sub   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_div   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_mul   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_sqrt  (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_push  (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_jmp   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_je    (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_jne   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_ja    (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_jae   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_jb    (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_jbe   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_pushr (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_popr  (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_call  (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_ret   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);
spu_err_t   spu_hlt   (stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs);

//————————————————————————————————————————————————————————————————————————————————

#endif //SPU.H