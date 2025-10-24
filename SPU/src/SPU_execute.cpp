#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t spu_execute_instructions (spu_context_t* spu_context)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    for (int pos = 0; pos < spu_context->bytecode_size; pos++)
    {
        int idx = (spu_context->bytecode)[pos];
        
        if (idx == HLT)
        {
            stack_destroy (&spu_context->return_labels);
            return SPU_SUCCESS;
        }
        
        spu_err_t status = commands_table[idx](spu_context, &pos);
        
        if (status)
            return status; 
    }

    PRINTERR (NO HALT IN COMMANDS);

    return SPU_LACK_OF_HALT;
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