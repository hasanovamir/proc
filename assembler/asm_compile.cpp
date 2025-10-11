#include "assembler.h"

//--------------------------------------------------------------------------------

error_t compile (context_t* context, int* labels)
{
    DEBUG_ASSERT (context                              != NULL);
    DEBUG_ASSERT (context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (context->bytecode_container.bytecode != NULL);

    label_t      labels[10]          = {};
    fixup_t      fixup_table_arr[20] = {};
    fixup_list_t fixup_list          = {.fixup_table = fixup_table_arr};

    for (int i = 0, int pos = 1; i < context->parsed_lines.num_lines; i++)
    {
        char tmp[10] = "";
        
        sscanf (context->parsed_lines.line_array[i], "%s", tmp);

        if (strcmp (tmp, "PUSH") == 0)
        {
            push_op (context, &pos, i);
        }
        else if (strcmp (tmp, "OUT") == 0)
        {
            out_op (context, &pos);
        }
        else if (strcmp (tmp, "ADD") == 0)
        {
            add_op (context, &pos);
        }
        else if (strcmp (tmp, "SUB") == 0)
        {
            sub_op (context, &pos);
        }
        else if (strcmp (tmp, "MUL") == 0)
        {
            mul_op (context, &pos);
        }
        else if (strcmp (tmp, "DIV") == 0)
        {
            div_op (context, &pos);
        }
        else if (strcmp (tmp, "SQRT") == 0)
        {
            sqrt_op (context, &pos);
        }
        else if (strcmp (tmp, "HLT") == 0)
        {
            hlt_op (context, &pos);
        }
        else if (strcmp (tmp, "IN") == 0)
        {
            in_op (context, &pos);
        }
        else if (strcmp (tmp, "POPR") == 0)
        {
            popr_op (context, &pos);
        }
        else if (strcmp (tmp, "PUSHR") == 0)
        {
            pushr_op (context, &pos);
        }
        else if (strcmp (tmp, "JMP") == 0)
        {
            context->bytecode_container.bytecode[pos++] = 10;

            int pos_to_jump = 0;

            if (context->parsed_lines.line_array[i][4] == ':')
            {
                sscanf (context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jump);
                context->bytecode_container.bytecode[pos++] = labels[pos_to_jump - 1];
            }
            

            context->bytecode_container.num_bytecode_elements += 2;
            context->bytecode_container.num_commands++;

        }
        else if (strcmp (tmp, "JE") == 0)
        {
            int pos_to_jump = 0;

            sscanf (context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jump);
            
            context->bytecode_container.bytecode[pos++] = 11;
            context->bytecode_container.bytecode[pos++] = labels[pos_to_jump - 1];

            context->bytecode_container.num_bytecode_elements += 2;
            context->bytecode_container.num_commands++;

        }
        else if (strcmp (tmp, "JNE") == 0)
        {
            int pos_to_jump = 0;

            sscanf (context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jump);
            
            context->bytecode_container.bytecode[pos++] = 12;
            context->bytecode_container.bytecode[pos++] = labels[pos_to_jump - 1];

            context->bytecode_container.num_bytecode_elements += 2;
            context->bytecode_container.num_commands++;

        }
        else if (strcmp (tmp, "JA") == 0)
        {
            int pos_to_jump = 0;

            sscanf (context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jump);
            
            context->bytecode_container.bytecode[pos++] = 13;
            context->bytecode_container.bytecode[pos++] = labels[pos_to_jump - 1];

            context->bytecode_container.num_bytecode_elements += 2;
            context->bytecode_container.num_commands++;

        }
        else if (strcmp (tmp, "JAE") == 0)
        {
            int pos_to_jump = 0;

            sscanf (context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jump);
            
            context->bytecode_container.bytecode[pos++] = 14;
            context->bytecode_container.bytecode[pos++] = labels[pos_to_jump - 1];

            context->bytecode_container.num_bytecode_elements += 2;
            context->bytecode_container.num_commands++;

        }
        else if (strcmp (tmp, "JB") == 0)
        {
            int pos_to_jump = 0;

            sscanf (context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jump);
            
            context->bytecode_container.bytecode[pos++] = 15;
            context->bytecode_container.bytecode[pos++] = labels[pos_to_jump - 1];

            context->bytecode_container.num_bytecode_elements += 2;
            context->bytecode_container.num_commands++;

        }
        else if (strcmp (tmp, "JBE") == 0)
        {
            int pos_to_jump = 0;

            sscanf (context->parsed_lines.line_array[i] + 3, "%d", &pos_to_jump);
            
            context->bytecode_container.bytecode[pos++] = 16;
            context->bytecode_container.bytecode[pos++] = labels[pos_to_jump - 1];

            context->bytecode_container.num_bytecode_elements += 2;
            context->bytecode_container.num_commands++;

        }
        else if (context->parsed_lines.line_array[i][0] == ':')
        {
            int label_pos = 0;
            int label_val = 0;

            sscanf (context->parsed_lines.line_array[i] + 1, "%d", &label_pos);
            
            label_val = context->bytecode_container.num_bytecode_elements + 2;

            labels[label_pos - 1] = label_val;
        }
        else
        {
            fprintf (stderr, "UNKNOWN COMMAND\n");

            return ASM_ERR_INVALID_SCAN_SYM;
        }
    }

    context->bytecode_container.bytecode[0] = context->bytecode_container.num_commands;
    context->bytecode_container.num_bytecode_elements++;

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

registers_t convert_name_of_reg_to_num (char* name)
{
    DEBUG_ASSERT (name != NULL);

    if (strcmp (name, "REG1") == 0)
        return REG1;
    else if (strcmp (name, "REG2") == 0)
        return REG2;
    else if (strcmp (name, "REG3") == 0)
        return REG3;
    else if (strcmp (name, "REG4") == 0)
        return REG4;
    else if (strcmp (name, "REG5") == 0)
        return REG5;
    else if (strcmp (name, "REG6") == 0)
        return REG6;
    else if (strcmp (name, "REG7") == 0)
        return REG7;
    else if (strcmp (name, "REG8") == 0)
        return REG8;
    else if (strcmp (name, "REG9") == 0)
        return REG9;
    else if (strcmp (name, "REG10") == 0)
        return REG10;
    else if (strcmp (name, "REG11") == 0)
        return REG11;
    else if (strcmp (name, "REG12") == 0)
        return REG12;
    else if (strcmp (name, "REG13") == 0)
        return REG13;
    else if (strcmp (name, "REG14") == 0)
        return REG14;
    else if (strcmp (name, "REG15") == 0)
        return REG15;
    else if (strcmp (name, "REG16") == 0)
        return REG16;

    return ERROR;
}

//--------------------------------------------------------------------------------