#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t spu_push (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    if (stack_push (stk, spu_context->command_array[*pos]) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
            
        return SPU_ALLOCATION_ERR;
    }
        
    spu_context->num_completed_commands++;
    

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_out (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int a = 0;

    stack_err_t status = STACK_SUCCESS;

    if ((status = stack_pop (stk, &a)) == STACK_ALLOCATION_ERR)
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

    printf ("%d\n", a);

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_add (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_push (stk, a + b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_sub (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_push (stk, b - a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_div (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_push (stk, b / a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_mul (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_push (stk, a * b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_sqrt (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int a = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    a = (stack_val_t)sqrt (a);

    if (stack_push (stk, a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_in (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int a = 0;

    scanf ("%d", &a);

    if (stack_push (stk, a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_popr (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    b = spu_context->command_array[*pos];
    
    reg[b] = a;

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_pushr (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int a = 0, b = 0;

    b = spu_context->command_array[*pos];

    a = reg[b];

    if (stack_push (stk, a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    spu_context->num_completed_commands++;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_jmp (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    *pos = new_pos - 1; 

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_je (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (b == a)
    {
        *pos = new_pos - 1;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_jne (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (b != a)
    {
        *pos = new_pos - 1;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_ja (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (b > a)
    {
        *pos = new_pos - 1;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------
spu_err_t spu_jae (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (b >= a)
    {
        *pos = new_pos - 1;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------
spu_err_t spu_jb (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (b < a)
    {
        *pos = new_pos - 1;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------
spu_err_t spu_jbe (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    int a = 0, b = 0;

    if (stack_pop (stk, &a) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (stack_pop (stk, &b) == STACK_ALLOCATION_ERR)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    if (b <= a)
    {
        *pos = new_pos - 1;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_call (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    stack_push (return_labels, (*pos) + 2);

    (*pos)++;

    int new_pos = spu_context->command_array[*pos];

    *pos = new_pos - 1; 

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_ret (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    DEBUG_ASSERT (stk           != NULL);
    DEBUG_ASSERT (spu_context   != NULL);
    DEBUG_ASSERT (pos           != NULL);
    DEBUG_ASSERT (return_labels != NULL);
    DEBUG_ASSERT (reg           != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    int new_pos = 0;

    stack_pop (return_labels, &new_pos);

    *pos = new_pos - 1;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_hlt (stack_t* stk, spu_context_t* spu_context, int* pos, stack_t* return_labels, int* reg)
{
    return SPU_HLT;
}

//--------------------------------------------------------------------------------