#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t spu_execute_instructions (spu_context_t* spu_context, stack_t* stk, int* reg)
{
    DEBUG_ASSERT (spu_context                != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    stack_t     return_labels;
    stack_err_t status = stack_init (&return_labels, 20);

    if (status)
        return SPU_ALLOCATION_ERR;

    spu_err_t (*commands_table[43])(stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* reg) = {};

    fill_command_table (commands_table);

    for (int pos = 0; pos < spu_context->command_array_size; pos++)
    {
        spu_err_t status = commands_table[(spu_context->command_array)[pos]](stk, spu_context, &pos, &return_labels, reg);

        if (status)
            return status;
    }

    fprintf (stderr, "NO HALT IN COMMANDS");

    return SPU_LACK_OF_HALT;
}

//--------------------------------------------------------------------------------

void fill_command_table (spu_err_t (** commands_table)(stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* reg))
{
    commands_table[PUSH]    = &spu_push ;
    commands_table[OUT]     = &spu_out  ;
    commands_table[ADD]     = &spu_add  ;
    commands_table[SUB]     = &spu_sub  ;
    commands_table[MUL]     = &spu_mul  ;
    commands_table[DIV]     = &spu_div  ;
    commands_table[SQRT]    = &spu_sqrt ;
    commands_table[HLT]     = &spu_hlt  ;
    commands_table[IN]      = &spu_in   ;
    commands_table[JMP]     = &spu_jmp  ;
    commands_table[JE]      = &spu_je   ;
    commands_table[JNE]     = &spu_jne  ;
    commands_table[JA]      = &spu_ja   ;
    commands_table[JAE]     = &spu_jae  ;
    commands_table[JB]      = &spu_jb   ;
    commands_table[JBE]     = &spu_jbe  ;
    commands_table[CALL]    = &spu_call ;
    commands_table[RET]     = &spu_ret  ;
    commands_table[PUSHR]   = &spu_pushr;
    commands_table[POPR]    = &spu_popr ;
}

//--------------------------------------------------------------------------------

const char* error_spu_code_to_string (spu_err_t status)
{
    switch (status)
    {
        ERRCASE (SPU_SUCCESS       );
        ERRCASE (SPU_ALLOCATION_ERR);
        ERRCASE (SPU_OPEN_FILE_ERR );
        ERRCASE (SPU_FREAD_ERR     );
        ERRCASE (SPU_LACK_OF_HALT  );
        ERRCASE (SPU_SCAN_ERR      );
        ERRCASE (SPU_NO_EL_TO_POP  );
        default:
            return "SPU_SUCCESS";
    }
}

//--------------------------------------------------------------------------------