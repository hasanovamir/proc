#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "assembler.h"

//--------------------------------------------------------------------------------

void push_op (asm_context_t* asm_context, int* pos, int i, asm_operation_t operation)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    int value = 0;

    long long offset = 1 + strchr (asm_context->parsed_lines.line_array[i], 'H') - asm_context->parsed_lines.line_array[i];

    sscanf (asm_context->parsed_lines.line_array[i] + offset, "%d", &value);

    asm_context->bytecode_container.bytecode[(*pos)++] = PUSH;

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

    char tmp[10] = "";

    int register_num = 0;

    long long offset = 1 + strchr (asm_context->parsed_lines.line_array[i], 'R') - asm_context->parsed_lines.line_array[i];
    
    int read_arg_count = sscanf (asm_context->parsed_lines.line_array[i] + offset, "%3s %d", tmp, &register_num);

    if (strcmp (tmp, "REG") != 0 || read_arg_count != 2)
    {
        PRINTERR (ASM_ERR_UNKNOWN_REG_NAME);
    }

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

    int label_pos = 0;
    int label_num = 0;

    long long offset = 1 + strchr (asm_context->parsed_lines.line_array[i], ':') - asm_context->parsed_lines.line_array[i];

    sscanf (asm_context->parsed_lines.line_array[i] + offset, "%d", &label_num);
            
    label_pos = asm_context->bytecode_container.num_bytecode_elements - 1;

    asm_context->labels[label_num - 1] = label_pos;
}

//--------------------------------------------------------------------------------

void upsize_fixup_context_if_need (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->fixup_context.fixup_table      != NULL);

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

    char tmp[10] = "";

    int register_num = 0;

    char opening_bracket = 0, closing_bracket = 0;

    long long offset = 1 + strchr (asm_context->parsed_lines.line_array[i], 'M') - asm_context->parsed_lines.line_array[i];
    
    sscanf (asm_context->parsed_lines.line_array[i] + offset + 1, "%c %3s %d %c", &opening_bracket, tmp, &register_num, &closing_bracket);

    if (strcmp (tmp, "REG") != 0 || opening_bracket != '[' || closing_bracket != ']')
    {
        PRINTERR (ASM_ERR_UNKNOWN_REG_NAME);
    }

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

    long long offset = 1 + strchr (asm_context->parsed_lines.line_array[i], ':') - asm_context->parsed_lines.line_array[i];
    
    int input_label_num = 0;

    sscanf (asm_context->parsed_lines.line_array[i] + offset, "%d", &input_label_num);

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