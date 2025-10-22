#include "assembler.h"

//--------------------------------------------------------------------------------

void fixup_labels (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                          != NULL);
    DEBUG_ASSERT (asm_context->fixup_context.fixup_table  != NULL);

    for (int i = 0; i < asm_context->fixup_context.fixup_count; i++)
    {
        int label_number = asm_context->fixup_context.fixup_table[i].label_num;
        int label_index  = asm_context->fixup_context.fixup_table[i].index;

        int new_pointer  = asm_context->labels[label_number - 1];

        asm_context->bytecode_container.bytecode[label_index] = new_pointer;
    }
}

//--------------------------------------------------------------------------------

void assemble (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    fixup_t* fixup_array = (fixup_t*)calloc (fixup_array_size, sizeof (fixup_t));

    asm_context->fixup_context = {.fixup_table = fixup_array, .fixup_capacity = fixup_array_size};
    
    for (int i = 0, pos = 0; i < asm_context->parsed_lines.num_lines; i++)
    {
        if (check_is_line_empty (asm_context, i))
            continue;

        char tmp[10] = "";

        sscanf (asm_context->parsed_lines.line_array[i], "%s", tmp);

        asm_operation_t idx = get_oper_idx (tmp);

        operations_table[idx](asm_context, &pos, i, idx);
    }

    fixup_labels (asm_context);

    free (fixup_array);
}


//--------------------------------------------------------------------------------

asm_operation_t get_oper_idx (const char* input_oper)
{
    int size = sizeof (oper_name_and_idx) / sizeof (oper_name_and_idx_t);

    for (int i = 0; i < size; i++)
        if (strcmp (input_oper, oper_name_and_idx[i].name) == 0)
            return oper_name_and_idx[i].idx;

    if (input_oper[0] == ':')
        return FILL_LABELS;

    fprintf (stderr, "UNKNOWN COMMAND\n");

    return ERR;
}

//--------------------------------------------------------------------------------

int check_is_line_empty (asm_context_t* asm_context, int i)
{
    DEBUG_ASSERT (asm_context != NULL);

    int j = 0;
    char* str = asm_context->parsed_lines.line_array[i];

    while (1)
    {
        if (isalpha (str[j]) || str[j] == ':')
            return false;
        if (str[j] == '\n' || str[j] == ';' || str[j] == '\0')
            return true;
        j++;
    }
}

//--------------------------------------------------------------------------------