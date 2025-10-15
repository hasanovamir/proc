#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "assembler.h"

//--------------------------------------------------------------------------------

void push_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    int value = 0;

    sscanf (asm_context->parsed_lines.line_array[i] + 4, "%d", &value);

    asm_context->bytecode_container.bytecode[(*pos)++] = 1;
    asm_context->bytecode_container.bytecode[(*pos)++] = value;

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void out_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 2;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void add_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 3;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------


void sub_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 4;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void mul_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 5;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void div_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 6;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void sqrt_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 7;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void hlt_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 8;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void in_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 9;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void popr_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    char tmp[10] = "";

    sscanf (asm_context->parsed_lines.line_array[i] + 4, "%s", tmp);

    int num_reg = convert_reg_name_to_num (tmp);
            
    if (num_reg == -1)
    {
        fprintf (stderr, "%s:%d %s\nsegmentation fault\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
        exit (1);
    }

    asm_context->bytecode_container.bytecode[(*pos)++] = 42;
    asm_context->bytecode_container.bytecode[(*pos)++] = num_reg;

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void pushr_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    char tmp[10] = "";

    sscanf (asm_context->parsed_lines.line_array[i] + 5, "%s", tmp);

    int num_reg = convert_reg_name_to_num (tmp);
            
    if (num_reg == -1)
    {
        fprintf (stderr, "%s:%d %s\nsegmentation fault\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
        exit (1);
    }

    asm_context->bytecode_container.bytecode[(*pos)++] = 33;
    asm_context->bytecode_container.bytecode[(*pos)++] = num_reg;

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void jmp_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][4] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 5, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 10;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 10;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void je_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][3] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 4, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 11;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 11;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void jne_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][4] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 5, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 12;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 12;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void ja_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][3] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 4, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 13;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 13;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void jae_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][4] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 5, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 14;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 14;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void jb_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][3] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 4, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 15;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 15;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void jbe_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][4] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 5, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 16;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 16;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void fill_labels (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    int label_pos = 0;
    int label_num = 0;

    sscanf (asm_context->parsed_lines.line_array[i] + 1, "%d", &label_num);
            
    label_pos = asm_context->bytecode_container.num_bytecode_elements;

    labels[label_num - 1] = label_pos;
}

//--------------------------------------------------------------------------------

void call_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    if (asm_context->parsed_lines.line_array[i][5] == ':')
    {
        int input_label_num = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 6, "%d", &input_label_num);

        asm_context->bytecode_container.bytecode[(*pos)++] = 17;

        fixup_list->fixup_table[fixup_list->fixup_count].index = *pos;
        fixup_list->fixup_table[fixup_list->fixup_count++].label_num = input_label_num;

        asm_context->bytecode_container.bytecode[(*pos)++] = -1;
    }
    else
    {
        int pos_to_jmp = 0;

        sscanf (asm_context->parsed_lines.line_array[i] + 4, "%d", &pos_to_jmp);

        asm_context->bytecode_container.bytecode[(*pos)++] = 17;
        asm_context->bytecode_container.bytecode[(*pos)++] = pos_to_jmp;
    }

    asm_context->bytecode_container.num_bytecode_elements += 2;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void ret_op (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
    asm_context->bytecode_container.bytecode[(*pos)++] = 18;
    asm_context->bytecode_container.num_bytecode_elements++;
    asm_context->bytecode_container.num_commands++;
}

//--------------------------------------------------------------------------------

void skip_comment (asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels)
{
}

//--------------------------------------------------------------------------------