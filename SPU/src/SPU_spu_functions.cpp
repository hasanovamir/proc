#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t spu_push (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    if (stack_push (&spu_context->stk, spu_context->bytecode[*pos]) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
            
        return SPU_ALLOCATION_ERR;
    }
        
    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_out (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    spu_data_t value = 0;

    stack_err_t status = STACK_SUCCESS;

    if ((status = stack_pop (&spu_context->stk, &value)) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (status == STACK_NO_EL_TO_POP)
    {
        fprintf(stderr, "NO_EL_TO_POP in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_NO_EL_TO_POP;
    }

    printf ("%d\n", value);

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_add (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (stack_push (&spu_context->stk, last_el + second_last_el) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_sub (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (stack_push (&spu_context->stk, second_last_el - last_el) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_div (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk)

    if (stack_push (&spu_context->stk, second_last_el / last_el) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_mul (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (stack_push (&spu_context->stk, last_el * second_last_el) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_sqrt (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    spu_data_t last_el = 0;

    if (stack_pop (&spu_context->stk, &last_el) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    last_el = (spu_data_t)sqrt (last_el);

    if (stack_push (&spu_context->stk, last_el) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_in (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    spu_data_t new_last_el = 0;

    scanf ("%d", &new_last_el);

    if (stack_push (&spu_context->stk, new_last_el) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_popr (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    spu_data_t reg_num = 0, value = 0;

    if (stack_pop (&spu_context->stk, &value) == STACK_ALLOCATION_ERR)
    {
        PRINTERR (SPU_ALLOCATION_ERR);

        return SPU_ALLOCATION_ERR;
    }

    reg_num = spu_context->bytecode[*pos];
    
    spu_context->reg[reg_num] = value;

    spu_context->num_completed_commands++;


    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_pushr (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    spu_data_t reg_num = 0, value = 0;

    reg_num = spu_context->bytecode[*pos];

    value = spu_context->reg[reg_num];

    if (stack_push (&spu_context->stk, value) == STACK_ALLOCATION_ERR)
    {
        PRINTERR (SPU_ALLOCATION_ERR);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_jmp (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    *pos = new_pos; 

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_je (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (last_el == second_last_el)
    {
        *pos = new_pos;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_jne (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (second_last_el != last_el)
    {
        *pos = new_pos;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_ja (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (second_last_el > last_el)
    {
        *pos = new_pos;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------
spu_err_t spu_jae (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    spu_data_t last_el = 0, second_last_el = 0;

   POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (second_last_el >= last_el)
    {
        *pos = new_pos;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------
spu_err_t spu_jb (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (second_last_el < last_el)
    {
        *pos = new_pos;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------
spu_err_t spu_jbe (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    spu_data_t last_el = 0, second_last_el = 0;

    POP_TWICE (&last_el, &second_last_el, &spu_context->stk);

    if (second_last_el <= last_el)
    {
        *pos = new_pos;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_call (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    stack_push (&spu_context->return_labels, (*pos) + 1);

    (*pos)++;

    int new_pos = spu_context->bytecode[*pos];

    *pos = new_pos; 

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_ret (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    int new_pos = 0;

    stack_pop (&spu_context->return_labels, &new_pos);

    *pos = new_pos;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_pushm (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int reg_number = spu_context->bytecode[*pos];

    int rem_pos = spu_context->reg[reg_number];

    spu_data_t value = spu_context->ram[rem_pos];

    if (stack_push (&spu_context->stk, value) == STACK_ALLOCATION_ERR)
    {
        PRINTERR (SPU_ALLOCATION_ERR);

        return SPU_ALLOCATION_ERR;
    }

     spu_context->num_completed_commands++;

     return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_popm (spu_context_t* spu_context, int* pos)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (pos                   != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    (*pos)++;

    int reg_number = spu_context->bytecode[*pos];

    int rem_pos = spu_context->reg[reg_number];

    spu_data_t value = 0;

    if (stack_pop (&spu_context->stk, &value) == STACK_ALLOCATION_ERR)
    {
        PRINTERR (SPU_ALLOCATION_ERR);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->ram[rem_pos] = value;

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_draw (spu_context_t* spu_context, int* pos)
{
    for (int y = 0; y < (int)sqrt (ram_size); y++)
    {
        for (int x = 0; x < (int)sqrt (ram_size); x++)
        {
            if (*(spu_context->ram + y * (int)sqrt (ram_size) + x) == 0)
                printf (COLOR_RED "|%d|" COLOR_RESET, *(spu_context->ram + y * (int)sqrt (ram_size) + x));
            else
                printf (COLOR_GREEN "|%d|" COLOR_RESET, *(spu_context->ram + y * (int)sqrt (ram_size) + x));
        }
        printf ("\n");
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_meow (spu_context_t* spu_context, int* pos)
{
    printf ("MEOW");

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------