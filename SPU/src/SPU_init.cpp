#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t spu_init (spu_context_t* spu_context, const char* file_name, stack_t* stk)
{
    DEBUG_ASSERT (spu_context != NULL);
    DEBUG_ASSERT (file_name   != NULL);
    DEBUG_ASSERT (stk         != NULL);

    if (stack_init (stk, 100))
    {   
        return SPU_ALLOCATION_ERR;
    }

    if (allocation_command_array (spu_context, file_name))
    {
        return SPU_ALLOCATION_ERR;   
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t allocation_command_array (spu_context_t* spu_context, const char* file_name)
{
    DEBUG_ASSERT (spu_context != NULL);
    DEBUG_ASSERT (file_name   != NULL);

    
    spu_context->command_array_size = get_file_size (file_name) / 4;

    if (spu_context->command_array_size == -1)
    {
        return SPU_OPEN_FILE_ERR;
    }

    spu_context->command_array = (int*)calloc (spu_context->command_array_size, sizeof (int));
    if (spu_context->command_array == NULL)
    {
        fprintf(stderr, "SPU_ALLOCATION_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return SPU_ALLOCATION_ERR;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_destroy_command_array (spu_context_t* spu_context, stack_t* stk)
{
    DEBUG_ASSERT (spu_context          != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);

    stack_destroy  (stk);
    free (spu_context->command_array);

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------