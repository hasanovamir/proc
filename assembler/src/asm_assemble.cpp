#include "assembler.h"

//--------------------------------------------------------------------------------

void fixup_labels (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                             != NULL);
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

asm_error_t assemble (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);
    
    long long iteration_count = asm_context->parsed_lines.num_lines;
    long long oper_hash = 0;

    for (int i = 0, pos = 0; i < iteration_count; i++)
    {
        if (((i + 1) * 100) % iteration_count == 0)
            fprintf (stderr, "%d\n", ((i + 1) * 100) / iteration_count);

        oper_hash = count_hash (asm_context->parsed_lines.line_array[i]);
        
        asm_operation_t idx = get_oper_idx (oper_hash);

        if (idx == ERR)
            return ASM_ERR_UNKNOWN_COMMAND; 

        operations_table[idx](asm_context, &pos, i, idx);
    }

    fixup_labels (asm_context);

    fprintf (stderr, "assembly is complete!\n");

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

asm_operation_t get_oper_idx (long long input_hash)
{
    inf_about_oper_t search_key = {"", (asm_operation_t) 0, input_hash};

    inf_about_oper_t* ptr = (inf_about_oper_t*) bsearch (&search_key, inf_about_oper, FUNC_COUNT, sizeof (inf_about_oper_t), &comp);
    
    if (ptr == NULL)
    {
        PRINTERR (ASM_ERR_UNKNOWN_COMMAND)

        return ERR;
    }

    return ptr->idx;
}

//--------------------------------------------------------------------------------

long long count_hash (const char* string)
{
    long long hash = 5381;
    int  i = 0;
    char c = 0;

    while (isalpha (c = string[i++]) || c == ':')
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

//--------------------------------------------------------------------------------