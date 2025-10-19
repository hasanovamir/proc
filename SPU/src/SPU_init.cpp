#include "stack.h"
#include "SPU.h"
#include "general_io_file_func.h"

//--------------------------------------------------------------------------------

spu_err_t spu_init (spu_context_t* spu_context, int argc, char** argv)
{
    DEBUG_ASSERT (spu_context != NULL);

    if (argc == 1)
    {
        PRINTERR (SPU_LACK_OF_FILE);

        return SPU_LACK_OF_FILE;
    }

    spu_context->file_name = argv[1];

    if (stack_init (&spu_context->stk, spu_stack_size))
    {   
        return SPU_ALLOCATION_ERR;
    }

    if (stack_init (&spu_context->return_labels, spu_ret_labels_stack_size))
    {   
        return SPU_ALLOCATION_ERR;
    }

    if (allocate_bytecode_array (spu_context))
    {
        return SPU_ALLOCATION_ERR;   
    }

    spu_context->ram = (spu_data_t*)calloc (ram_size, sizeof (spu_data_t));

    if (spu_context->ram == NULL)
    {
        PRINTERR (SPU_ALLOCATION_ERR);

        return SPU_ALLOCATION_ERR;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t allocate_bytecode_array (spu_context_t* spu_context)
{
    DEBUG_ASSERT (spu_context != NULL);
 
    spu_context->bytecode_size = get_file_size (spu_context->file_name);

    if ((spu_context->bytecode_size % 4) != 0)
    {
        PRINTERR (SPU_INCORRECT_FILE_SIZE);

        return SPU_INCORRECT_FILE_SIZE;
    } 

    spu_context->bytecode_size /= sizeof (spu_data_t);

    if (spu_context->bytecode_size == -1)
    {
        PRINTERR (SPU_OPEN_FILE_ERR);

        return SPU_OPEN_FILE_ERR;
    }

    spu_context->bytecode = (spu_data_t*)calloc (spu_context->bytecode_size, sizeof (spu_data_t));

    if (spu_context->bytecode == NULL)
    {
        PRINTERR (SPU_ALLOCATION_ERR);

        return SPU_ALLOCATION_ERR;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t spu_destroy (spu_context_t* spu_context)
{
    DEBUG_ASSERT (spu_context                != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    stack_destroy (&spu_context->stk);
    free          (spu_context->bytecode);
    free          (spu_context->ram);

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------