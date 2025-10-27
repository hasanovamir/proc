#include "assembler.h"

//--------------------------------------------------------------------------------

void push_op (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    char* ptr_to_arg = 1 + strchr (asm_context->parsed_lines.line_array[i], 'H');

    int value = atoi (ptr_to_arg);

    asm_context->bytecode_container.bytecode[(*pos)++] = operation;

    asm_context->bytecode_container.bytecode[(*pos)++] = value;

    asm_context->bytecode_container.num_commands++;

    asm_context->bytecode_container.num_bytecode_elements += 2;
}

//--------------------------------------------------------------------------------

void reg_arg_op (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    char* ptr_to_arg = 1 + strchr (asm_context->parsed_lines.line_array[i], 'G');

    int register_num = atoi (ptr_to_arg);

    asm_context->bytecode_container.bytecode[(*pos)++] = operation;

    asm_context->bytecode_container.bytecode[(*pos)++] = register_num - 1;

    asm_context->bytecode_container.num_commands++;

    asm_context->bytecode_container.num_bytecode_elements += 2;
}

//--------------------------------------------------------------------------------

void fill_labels (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    (void)pos;
    (void)operation;

    int label_pos = 0;
    int label_num = 0;

    char* ptr_to_arg = 1 + strchr (asm_context->parsed_lines.line_array[i], ':');

    label_num = atoi (ptr_to_arg);
            
    label_pos = asm_context->bytecode_container.num_bytecode_elements - 1;

    asm_context->labels[label_num - 1] = label_pos;
}

//--------------------------------------------------------------------------------

void upsize_fixup_context_if_need (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->fixup_context.fixup_table   != NULL);

    if (asm_context->fixup_context.fixup_count == asm_context->fixup_context.fixup_capacity - 1)
    {
        asm_context->fixup_context.fixup_table = (fixup_t*)realloc (asm_context->fixup_context.fixup_table, asm_context->fixup_context.fixup_capacity * 2);

        asm_context->fixup_context.fixup_capacity = asm_context->fixup_context.fixup_capacity * 2; 
    }
}

//--------------------------------------------------------------------------------

void ram_op (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    char* ptr_to_arg = - 1 + strchr (asm_context->parsed_lines.line_array[i], ']');

    int register_num = atoi (ptr_to_arg);

    asm_context->bytecode_container.bytecode[(*pos)++] = operation;

    asm_context->bytecode_container.bytecode[(*pos)++] = register_num - 1;

    asm_context->bytecode_container.num_commands++;

    asm_context->bytecode_container.num_bytecode_elements += 2;
}

//--------------------------------------------------------------------------------

void no_arg_op (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    (void)i;

    asm_context->bytecode_container.bytecode[(*pos)++] = operation;

    asm_context->bytecode_container.num_commands++;

    asm_context->bytecode_container.num_bytecode_elements++;
}

//--------------------------------------------------------------------------------

void jmp_core_op (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);

    char* ptr_to_arg = 1 + strchr (asm_context->parsed_lines.line_array[i], ':');
    
    int input_label_num = atoi (ptr_to_arg);

    asm_context->bytecode_container.bytecode[(*pos)++] = operation;

    fill_fixup_context (asm_context, pos, input_label_num);

    asm_context->bytecode_container.bytecode[(*pos)++] = -1;

    asm_context->bytecode_container.num_bytecode_elements += 2;

    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void fill_fixup_context (asm_context_t* asm_context, int* pos, int input_label_num)
{
    DEBUG_ASSERT (asm_context                            != NULL);
    DEBUG_ASSERT (asm_context->fixup_context.fixup_table != NULL);

    upsize_fixup_context_if_need (asm_context);

    asm_context->fixup_context.fixup_table[asm_context->fixup_context.fixup_count].index = *pos;

    asm_context->fixup_context.fixup_table[asm_context->fixup_context.fixup_count++].label_num = input_label_num;
}

//--------------------------------------------------------------------------------