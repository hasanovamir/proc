#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t read_bytecode (spu_context_t* spu_context)
{
    DEBUG_ASSERT (spu_context           != NULL);
    DEBUG_ASSERT (spu_context->bytecode != NULL);

    FILE* file = fopen (spu_context->file_name, "rb");
    
    if (file == NULL)
    {
        PRINTERR (SPU_OPEN_FILE_ERR);
   
        return SPU_OPEN_FILE_ERR;
    }

    int read_size = (int)fread (spu_context->bytecode, sizeof (spu_data_t), spu_context->bytecode_size, file);

    if (spu_context->bytecode_size != read_size)
    {
        PRINTERR (SPU_FREAD_ERR);

        return SPU_FREAD_ERR;
    }

    fclose (file);

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------