#include "assembler.h"

//--------------------------------------------------------------------------------

error_t allocate_el_arr (context_t* context, const char* file_name)
{
    DEBUG_ASSERT (context   != NULL);
    DEBUG_ASSERT (file_name != NULL);

    context->source_buffer.buffer_size = get_file_size (file_name);

    if (context->source_buffer.buffer_size == -1)
    {
        return ASM_ERR_OPEN_READ_FILE;
    }

    context->source_buffer.source_code_array = (char*)calloc (context->source_buffer.buffer_size + 1, sizeof (char));

    if (context->source_buffer.source_code_array == NULL)
    {
        fprintf(stderr, "ASM_ERR_ALLOCATION in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_ALLOCATION;
    }

    context->source_buffer.source_code_array[context->source_buffer.buffer_size] = '\0';

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t allocate_line_arr (context_t* context)
{
    DEBUG_ASSERT (context                                  != NULL);
    DEBUG_ASSERT (context->source_buffer.source_code_array != NULL);

    context->parsed_lines.line_array = (char**)calloc (context->parsed_lines.num_lines, sizeof (char*));

    if (context->parsed_lines.line_array == NULL)
    {
        fprintf(stderr, "ASM_ERR_ALLOCATION in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_ALLOCATION;
    }

    fill_line_array (context);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t allocate_com_arr (context_t* context)
{
    DEBUG_ASSERT (context != NULL);

    context->bytecode_container.bytecode = (int* )calloc (context->parsed_lines.num_lines * 2, sizeof (int));

    if (context->bytecode_container.bytecode == NULL)
    {
        fprintf(stderr, "ASM_ERR_ALLOCATION in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return ASM_ERR_ALLOCATION;
    }

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t count_n_lines (context_t* context)
{
    DEBUG_ASSERT (context                                 != NULL);
    DEBUG_ASSERT (context->source_buffer.source_code_array != NULL);

    int i = 0;
    char* tmp = context->source_buffer.source_code_array;

    while (tmp != NULL)
    {
        tmp = strchr (tmp, '\n');

        if (tmp != NULL)
        {
            i++;
            tmp++;
        }
    }

    context->parsed_lines.num_lines = i + 1;

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t fill_line_array (context_t* context)
{
    DEBUG_ASSERT (context                           != NULL);
    DEBUG_ASSERT (context->parsed_lines.line_array != NULL);

    char* tmp = context->source_buffer.source_code_array;

    for (int i = 0; i < context->parsed_lines.num_lines; i++)
    {
        (context->parsed_lines.line_array)[i]  = tmp;
        tmp = strchr (tmp, '\n') + 1;
    }

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t destroy_arrays (context_t* context)
{
    DEBUG_ASSERT (context                               != NULL);
    DEBUG_ASSERT (context->bytecode_container.bytecode != NULL);
    DEBUG_ASSERT (context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (context->source_buffer.source_code_array  != NULL);

    free (context->bytecode_container.bytecode);
    free (context->parsed_lines.line_array    );
    free (context->source_buffer.source_code_array );

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------