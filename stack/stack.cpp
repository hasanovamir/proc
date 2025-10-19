#include "..\stack.\stack.h"

//--------------------------------------------------------------------------------

stack_err_t stack_init (stack_t* stk, int init_capacity)
{
    DEBUG_ASSERT (stk != NULL);

    if (init_capacity < 1)
    {
        fprintf(stderr, "STACK_INVALID_VALUE in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return STACK_INVALID_VALUE;
    }
    
    int array_size = (init_capacity > array_min_size) ? init_capacity + 2: array_min_size;

    stk->data = (stack_val_t*)calloc (array_size, sizeof (stack_val_t));

    if (stk->data == NULL)
    {
        fprintf(stderr, "STACK_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        
        return STACK_ALLOCATION_ERR;
    }

    stk->capacity = array_size - 2;
    stk->size = 0;
    
    stack_fill (stk);

    STACK_OK (stk);

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t stack_push (stack_t* stk, stack_val_t value)
{
    STACK_OK (stk);

    if (upsize_array_if_need (stk))
    {
        fprintf(stderr, "STACK_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        
        return STACK_ALLOCATION_ERR;
    }

    stk->data[++stk->size] = value;

    STACK_OK (stk);

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t stack_pop (stack_t* stk, stack_val_t* val_pointer)
{
    STACK_OK (stk);
    if (stk->size == 0)
    {
        return STACK_NO_EL_TO_POP;
    }

    if (downsize_array_if_need (stk))
    {
        fprintf(stderr, "STACK_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        
        return STACK_ALLOCATION_ERR;
    }

    *val_pointer = stk->data[stk->size];
    stk->data[stk->size--] = POISON;

    STACK_OK (stk);
    
    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t stack_destroy (stack_t* stk)
{
    STACK_OK (stk);

    free (stk->data);

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

const char* error_stack_code_to_string (stack_err_t status)
{
    switch (status)
    {
        ERRCASE (STACK_DATA_RUINED   );
        ERRCASE (STACK_ALLOCATION_ERR);
        ERRCASE (STACK_NO_EL_TO_POP  );
        ERRCASE (STACK_SUCCESS       );
        ERRCASE (STACK_INVALID_VALUE );
        ERRCASE (STACK_SCAN_ERR      );
        ERRCASE (STACK_UNKNOWN_ERR   );
        default:
            return "SUCCESS";
    }
}

//-----------------------------------------------------------------------------

void stack_dump (stack_t* stk)
{
    printf ("Stack [%p]\n{\n", stk);

    int tmp = (stk->capacity < 100) ? stk->capacity : 100;

    printf ("\tsize = %d\n",     stk->size     );
    printf ("\tcapacity = %d\n", stk->capacity );
    printf ("\tdata[%p]\n\t{\n", stk->data     );

    if (stk->data[0] == CANARY)
    {
        printf ("\t\t[ 0] = CANARY\n");
    }
    else
    {
        printf ("\t\t[ 0] = %d (not CANARY)\n", stk->data[0]);
    }

    for (int i = 1; i < tmp + 1; i++)
    {
        if (stk->data[i] == POISON)
        {
            printf ("\t\t[%2d] = POISON\n", i);
        }
        else
        {
            printf ("\t\t*[%2d] = %d\n", i, stk->data[i]);
        }
    }

    if (stk->data[stk->capacity + 1] == CANARY)
    {
        printf ("\t\t[%2d] = CANARY\n", stk->capacity + 1);
    }
    else
    {
        printf ("\t\t[%2d] = %d (not CANARY)\n", stk->capacity + 1, stk->data[stk->capacity + 1]);
    }

    printf ("\t}\n}\n");
}

//--------------------------------------------------------------------------------

stack_err_t stack_verify (stack_t* stk)
{
    DEBUG_ASSERT (stk       != NULL);
    DEBUG_ASSERT (stk->data != NULL);

    if (stk->data[stk->capacity + 1] != CANARY || stk->data[0] != CANARY)
    {
        return STACK_DATA_RUINED;
    }

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t stack_fill (stack_t* stk)
{
    DEBUG_ASSERT (stk       != 0);
    DEBUG_ASSERT (stk->data != 0);

    stk->data[0]                 = CANARY;
    stk->data[stk->capacity + 1] = CANARY;

    for (int i = 1; i < stk->capacity + 1; i++)
    {
        stk->data[i] = POISON;
    }

    STACK_OK (stk);

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t upsize_array_if_need (stack_t* stk)
{
    DEBUG_ASSERT (stk       != NULL);
    DEBUG_ASSERT (stk->data != NULL);

    if (stk->size == stk->capacity - 1)
    {
        stack_val_t* old_data = stk->data;

        stk->data = (stack_val_t*)realloc (stk->data, stk->capacity * 2);

        if (stk->data == NULL)
        {
            stk->data = old_data;

            fprintf(stderr, "STACK_ALLOCATION_ERR in %s:%d func:%s\n",
                __FILE__, __LINE__, __PRETTY_FUNCTION__);

            return STACK_ALLOCATION_ERR;
        }

        stk->capacity = 2 * stk->capacity - 2;
    }

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t downsize_array_if_need (stack_t* stk)
{
    DEBUG_ASSERT (stk       != NULL);
    DEBUG_ASSERT (stk->data != NULL);

    if (stk->size <= stk->capacity / 4 && stk->capacity > array_min_size * 2)
    {
        stack_val_t* old_data = stk->data;

        stk->data = (stack_val_t*)realloc (stk->data, stk->capacity / 2);

        if (stk->data == NULL)
        {
            stk->data = old_data;

            fprintf(stderr, "STACK_ALLOCATION_ERR in %s:%d func:%s\n",
                __FILE__, __LINE__, __PRETTY_FUNCTION__);

            return STACK_ALLOCATION_ERR;
        }

        stk->capacity = stk->capacity / 2 - 2;
    }

    return STACK_SUCCESS;
}

//--------------------------------------------------------------------------------