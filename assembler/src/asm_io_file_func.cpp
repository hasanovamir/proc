#include "assembler.h"

//--------------------------------------------------------------------------------

asm_error_t read_source_code (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                                  != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array != NULL);

    const char* file_name = asm_context->read_file_name;

    FILE* file = fopen (file_name, "rb");

    if (file == NULL)
    {
        PRINTERR (ASM_ERR_OPEN_READ_FILE);

        return ASM_ERR_OPEN_READ_FILE;
    }
   
    asm_context->source_buffer.read_size = fread (asm_context->source_buffer.source_code_array, sizeof (char), asm_context->source_buffer.buffer_size, file);
    
    count_n_lines (asm_context);
    
    fclose (file);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

asm_error_t write_bytecode (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    const char* file_name = asm_context->write_file_name;
    
    FILE* file = fopen (file_name, "wb");

    if (file == NULL)
    {
        PRINTERR (ASM_ERR_OPEN_WRITE_FILE)
        ;
        return ASM_ERR_OPEN_WRITE_FILE;
    }

    fwrite (asm_context->bytecode_container.bytecode, sizeof (int), asm_context->bytecode_container.num_bytecode_elements, file);

    fclose (file);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

const char* error_code_to_string (asm_error_t status)
{
    switch (status)
    {
        ERRCASE (ASM_ERR_ALLOCATION      );
        ERRCASE (ASM_ERR_OPEN_READ_FILE  );
        ERRCASE (ASM_ERR_FREAD           );
        ERRCASE (ASM_ERR_LINE_ARR_FILL   );
        ERRCASE (ASM_ERR_INVALID_SCAN_SYM);
        ERRCASE (ASM_ERR_OPEN_WRITE_FILE );
        ERRCASE (ASM_SUCCESS             );
        ERRCASE (ASM_ERR_UNKNOWN_REG_NAME);
        default:
            return "ASM_SUCCESS";
    }
}

//--------------------------------------------------------------------------------