#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t spu_execute_instructions (spu_context_t* spu_context, stack_t* stk, int* reg)
{
    DEBUG_ASSERT (spu_context                != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    stack_t return_labels;
    stack_err_t status = stack_init (&return_labels, 20);

    if (status)
        return SPU_ALLOCATION_ERR;

    spu_err_t (*commands_table[43])(stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels) = {};

    for (int pos = 0; pos < spu_context->command_array_size; pos++)
    {
        switch ((spu_context->command_array[pos]))
        {
            case PUSH:
                if (spu_push (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case OUT:
                if (spu_out (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case ADD:
                if (spu_add (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case SUB:
                if (spu_sub (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case DIV:
                if (spu_div (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case MUL:
                if (spu_mul (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case SQRT:
                if (spu_sqrt (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case HLT:
                spu_context->num_completed_commands++;
                return SPU_SUCCESS;
            case IN:
                if (spu_in (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case POPR:
                if (spu_popr (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case PUSHR:
                if (spu_pushr (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case JMP:
                if (spu_jmp (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case JE:
                if (spu_je (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case JNE:
                if (spu_jne (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case JA:
                if (spu_ja (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case JAE:
                if (spu_jae (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case JB:
                if (spu_jb (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case JBE:
                if (spu_jbe (stk, spu_context, &pos, &return_labels, reg))
                {
                    return SPU_ALLOCATION_ERR;
                }
                break;
            case CALL:
                spu_call (stk, spu_context, &pos, &return_labels, reg);
                break;
            case RET:
                spu_ret (stk, spu_context, &pos, &return_labels, reg);
                break;
            default:
                fprintf (stderr, "SPU_SCAN_ERR in %s:%d in %s",
                    __FILE__, __LINE__, __PRETTY_FUNCTION__);
                return SPU_SCAN_ERR;
        }
    }

    fprintf (stderr, "NO HALT IN commandMANDS");

    return SPU_LACK_OF_HALT;
}

//--------------------------------------------------------------------------------

void fill_command_table (spu_err_t (** commands_table)(stack_t* stk, spu_context_t* spu_context_t, int* pos, stack_t* return_labels, int* regs))
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