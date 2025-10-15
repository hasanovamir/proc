#include "assembler.h"

//--------------------------------------------------------------------------------

int get_file_size (const char* file_name)
{
    DEBUG_ASSERT (file_name != NULL);

    struct stat statistic = {};
    stat (file_name, &statistic);

    int file_size = statistic.st_size;
    
    if (file_size == 0)
    {
        fprintf(stderr, "ERR_get_file_size in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return -1;
    }

    return file_size;
}

//--------------------------------------------------------------------------------

error_t read_commands (asm_context_t* asm_context, const char* file_name)
{
    DEBUG_ASSERT (asm_context                  != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array != NULL);
    DEBUG_ASSERT (file_name                 != NULL);

    FILE* file = fopen (file_name, "r");

    if (file == NULL)
    {
        fprintf(stderr, "ASM_ERR_OPEN_READ_FILE in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_OPEN_READ_FILE;
    }
   

    int read_size = (int)fread (asm_context->source_buffer.source_code_array, sizeof (char), asm_context->source_buffer.buffer_size, file);

    count_n_lines (asm_context);

    if (asm_context->source_buffer.buffer_size != read_size + asm_context->parsed_lines.num_lines - 1)
    {
        fprintf(stderr, "ASM_ERR_FREAD in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_FREAD;
    }

    fclose (file);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t write_commands (asm_context_t* asm_context, const char* file_name)
{
    DEBUG_ASSERT (asm_context  != 0);
    DEBUG_ASSERT (file_name != 0);
    
    FILE* file = fopen (file_name, "wb");

    if (file == NULL)
    {
        fprintf(stderr, "ASM_ERR_OPEN_WRITE_FILE in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_OPEN_WRITE_FILE;
    }

    fwrite (asm_context->bytecode_container.bytecode, sizeof (int), asm_context->bytecode_container.num_bytecode_elements, file);

    fclose (file);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

const char* error_code_to_string (error_t status)
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