#include "assembler.h"

//--------------------------------------------------------------------------------

static oper_name_and_idx_t oper_name_and_idx[] = 
{
    {"PUSH" , PUSH },
    {"OUT"  , OUT  },
    {"ADD"  , ADD  },
    {"SUB"  , SUB  },
    {"MUL"  , MUL  },
    {"DIV"  , DIV  },
    {"SQRT" , SQRT },
    {"HLT"  , HLT  },
    {"IN"   , IN   },
    {"PUSHR", PUSHR},
    {"POPR" , POPR },
    {"JMP"  , JMP  },
    {"JE"   , JE   },
    {"JNE"  , JNE  },
    {"JA"   , JA   },
    {"JAE"  , JAE  },
    {"JB"   , JB   },
    {"JBE"  , JBE  },
    {"CALL" , CALL },
    {"RET"  , RET  },
    
};

//--------------------------------------------------------------------------------

void fixup_labels (asm_context_t* asm_context, fixup_list_t* fixup_list, int* labels)
{
    DEBUG_ASSERT (asm_context != NULL);
    DEBUG_ASSERT (fixup_list  != NULL);

    for (int i = 0; i < fixup_list->fixup_count; i++)
    {
        int label_number = fixup_list->fixup_table[i].label_num;
        int label_index  = fixup_list->fixup_table[i].index;

        int new_pointer  = labels[label_number - 1];

        asm_context->bytecode_container.bytecode[label_index] = new_pointer;
    }
}

//--------------------------------------------------------------------------------

void compile (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                              != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array     != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode != NULL);

    void (*operations_table[43])(asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels) = {};
    
    fill_operation_table (operations_table);

    int labels[10] = {};
    fixup_t fixup_table_arr[20] = {};
    fixup_list_t fixup_list = {.fixup_table = fixup_table_arr};
    
    for (int i = 0, pos = 0; i < asm_context->parsed_lines.num_lines; i++)
    {
        char tmp[10] = "";
        sscanf (asm_context->parsed_lines.line_array[i], "%s", tmp);

        asm_operation_t idx = get_oper_idx (tmp);

        operations_table[idx](asm_context, &pos, i, &fixup_list, labels);
    }

    fixup_labels (asm_context, &fixup_list, labels);
}

//--------------------------------------------------------------------------------

void fill_operation_table (void (** operations_table)(asm_context_t* asm_context, int* pos, int i, fixup_list_t* fixup_list, int*labels))
{
    operations_table[PUSH]        = &push_op;
    operations_table[OUT]         = &out_op;
    operations_table[ADD]         = &add_op;
    operations_table[SUB]         = &sub_op;
    operations_table[MUL]         = &mul_op;
    operations_table[DIV]         = &div_op;
    operations_table[SQRT]        = &sqrt_op;
    operations_table[HLT]         = &hlt_op;
    operations_table[IN]          = &in_op ;
    operations_table[JMP]         = &jmp_op;
    operations_table[JE]          = &je_op ;
    operations_table[JNE]         = &jne_op;
    operations_table[JA]          = &ja_op ;
    operations_table[JAE]         = &jae_op;
    operations_table[JB]          = &jb_op ;
    operations_table[JBE]         = &jbe_op;
    operations_table[CALL]        = &call_op;
    operations_table[RET]         = &ret_op;
    operations_table[COMMENT]     = &skip_comment;
    operations_table[PUSHR]       = &pushr_op;
    operations_table[POPR]        = &popr_op;
    operations_table[FILL_LABELS] = &fill_labels;
}

//--------------------------------------------------------------------------------

asm_operation_t get_oper_idx (const char* input_oper)
{
    int size = sizeof (oper_name_and_idx) / 16;

    for (int i = 0; i < size; i++)
        if (strcmp (input_oper, oper_name_and_idx[i].name) == 0)
            return oper_name_and_idx[i].idx;

    if (input_oper[0] == ':')
        return FILL_LABELS;
    if (input_oper[0] == ';')
        return COMMENT;
    if (input_oper[0] == '\n')
        return COMMENT;

    fprintf (stderr, "UNKNOWN COMMAND\n");

    return ERR;
}

//--------------------------------------------------------------------------------

registers_t convert_reg_name_to_num (char* name)
{
    DEBUG_ASSERT (name != NULL);

    if (strcmp (name, "REG1") == 0)//здесь ж кринж наверное делать как в компеле
        return REG1;
    else if (strcmp (name, "REG2" ) == 0)
        return REG2;
    else if (strcmp (name, "REG3" ) == 0)
        return REG3;
    else if (strcmp (name, "REG4" ) == 0)
        return REG4;
    else if (strcmp (name, "REG5" ) == 0)
        return REG5;
    else if (strcmp (name, "REG6" ) == 0)
        return REG6;
    else if (strcmp (name, "REG7" ) == 0)
        return REG7;
    else if (strcmp (name, "REG8" ) == 0)
        return REG8;
    else if (strcmp (name, "REG9" ) == 0)
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