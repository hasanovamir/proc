#include "assembler.h"
#include "general_io_file_func.h"

//--------------------------------------------------------------------------------

error_t allocate_el_arr (asm_context_t* asm_context, int argc, char** argv)
{
    DEBUG_ASSERT (asm_context   != NULL);

    if (argc != 3)
        return ASM_INCORRECT_FILE_NUM;

    const char* read_file_name = argv[1];
    const char* open_file_name = argv[2];

    asm_context->read_file_name  = read_file_name;
    asm_context->write_file_name = open_file_name;

    asm_context->source_buffer.buffer_size = get_file_size (read_file_name);

    if (asm_context->source_buffer.buffer_size == -1)
    {
        return ASM_ERR_OPEN_READ_FILE;
    }

    asm_context->source_buffer.source_code_array = (char*)calloc (asm_context->source_buffer.buffer_size + 1, sizeof (char));

    if (asm_context->source_buffer.source_code_array == NULL)
    {
        fprintf(stderr, "ASM_ERR_ALLOCATION in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_ALLOCATION;
    }

    asm_context->source_buffer.source_code_array[asm_context->source_buffer.buffer_size] = '\0';

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t allocate_line_arr (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                                  != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array != NULL);

    asm_context->parsed_lines.line_array = (char**)calloc (asm_context->parsed_lines.num_lines, sizeof (char*));

    if (asm_context->parsed_lines.line_array == NULL)
    {
        fprintf(stderr, "ASM_ERR_ALLOCATION in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_ALLOCATION;
    }

    fill_line_array (asm_context);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t allocate_bytecode_array (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context != NULL);

    asm_context->bytecode_container.bytecode = (int* )calloc (asm_context->parsed_lines.num_lines * 2, sizeof (int));

    if (asm_context->bytecode_container.bytecode == NULL)
    {
        fprintf(stderr, "ASM_ERR_ALLOCATION in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_ALLOCATION;
    }

    memset (asm_context->labels, -1, labels_size * sizeof (int));

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t count_n_lines (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                                  != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array != NULL);

    int i = 0;
    char* tmp = asm_context->source_buffer.source_code_array;

    while (tmp != NULL)
    {
        tmp = strchr (tmp, '\n');

        if (tmp != NULL)
        {
            i++;
            tmp++;
        }
    }

    asm_context->parsed_lines.num_lines = i + 1;

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t fill_line_array (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                           != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array != NULL);

    char* tmp = asm_context->source_buffer.source_code_array;

    for (int i = 0; i < asm_context->parsed_lines.num_lines; i++)
    {
        (asm_context->parsed_lines.line_array)[i]  = tmp;
        tmp = strchr (tmp, '\n') + 1;
    }

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t asm_destroy (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                               != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array  != NULL);

    free (asm_context->bytecode_container.bytecode);
    free (asm_context->parsed_lines.line_array    );
    free (asm_context->source_buffer.source_code_array );

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------