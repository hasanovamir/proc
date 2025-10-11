#include "assembler.h"

//--------------------------------------------------------------------------------

void push_op (context_t* context, int* pos, int i)
{
    int value = 0;

    sscanf (context->parsed_lines.line_array[i] + 4, "%d", &value);

    context->bytecode_container.bytecode[(*pos)++] = 1;
    context->bytecode_container.bytecode[(*pos)++] = value;

    context->bytecode_container.num_bytecode_elements += 2;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void out_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 2;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void add_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 3;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------


void sub_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 4;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void mul_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 5;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void div_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 6;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void sqrt_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 7;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void hlt_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 8;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void in_op (context_t* context, int* pos)
{
    context->bytecode_container.bytecode[(*pos)++] = 9;
    context->bytecode_container.num_bytecode_elements++;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void popr_op (context_t* context, int* pos, int i)
{
    char tmp[10] = "";

    sscanf (context->parsed_lines.line_array[i] + 4, "%s", tmp);

    int num_reg = convert_name_of_reg_to_num (tmp);
            
    if (num_reg == -1)
    {
        return ASM_ERR_UNKNOWN_REG_NAME;
    }

    context->bytecode_container.bytecode[(*pos)++] = 42;
    context->bytecode_container.bytecode[(*pos)++] = num_reg;

    context->bytecode_container.num_bytecode_elements += 2;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void pushr_op (context_t* context, int* pos, int i)
{
    char tmp[10] = "";

    sscanf (context->parsed_lines.line_array[i] + 5, "%s", tmp);

    int num_reg = convert_name_of_reg_to_num (tmp);
            
    if (num_reg == -1)
    {
        return ASM_ERR_UNKNOWN_REG_NAME;
    }

    context->bytecode_container.bytecode[(*pos)++] = 33;
    context->bytecode_container.bytecode[(*pos)++] = num_reg;

    context->bytecode_container.num_bytecode_elements += 2;
    context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void jmp_op (context_t* context, int* pos, int i, fixup_list_t* fixup_list)
{
    if (context->source_buffer.source_code_array[i][4] == ':')
    {
        int num_label = 0;

        sscanf (context->source_buffer.source_code_array[i] + 5, "%d", &num_label);

        context->bytecode_container.bytecode[(*pos)++] = 10;

        fixup_list->fixup_table[fixup_list->fixup_count]->index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++]->label_num = num_label;

        context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (context->source_buffer.source_code_array[i] + 3, "%d", &pos_to_jmp);

        context->bytecode_container.bytecode[(*pos)++] = 10;
        context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    context->bytecode_container.num_bytecode_elements += 2;
    context->bytecode_container.num_commands++;
}