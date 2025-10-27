
#include "assembler.h"
#include "general_io_file_func.h"

//--------------------------------------------------------------------------------

inf_about_oper_t inf_about_oper[] = 
{
    {"PUSH" , PUSH , 0},
    {"OUT"  , OUT  , 0},
    {"ADD"  , ADD  , 0},
    {"SUB"  , SUB  , 0},
    {"MUL"  , MUL  , 0},
    {"DIV"  , DIV  , 0},
    {"SQRT" , SQRT , 0},
    {"HLT"  , HLT  , 0},
    {"IN"   , IN   , 0},
    {"JMP"  , JMP  , 0},
    {"JE"   , JE   , 0},
    {"JNE"  , JNE  , 0},
    {"JA"   , JA   , 0},
    {"JAE"  , JAE  , 0},
    {"JB"   , JB   , 0},
    {"JBE"  , JBE  , 0},
    {"CALL" , CALL , 0},
    {"RET"  , RET  , 0},
    {"PUSHR", PUSHR, 0},
    {"POPR" , POPR , 0},
    {"PUSHM", PUSHM, 0},
    {"POPM" , POPM , 0},
    {"DRAW" , DRAW , 0},
    {"MEOW" , MEOW , 0},
    {"DBA"  , DBA  , 0},
    {":"    , FILL_LABELS, 0}
};

//--------------------------------------------------------------------------------

asm_error_t asm_init (asm_context_t* asm_context, int argc, char** argv)
{
    DEBUG_ASSERT (asm_context != NULL);
    
    if (argc != 3)
    {
        PRINTERR (ASM_INCORRECT_FILE_NAME);

        return ASM_INCORRECT_FILE_NAME;
    }

    const char* read_file_name = argv[1];
    const char* open_file_name = argv[2];

    asm_context->read_file_name  = read_file_name;
    asm_context->write_file_name = open_file_name;

    for (int i = 0; i < FUNC_COUNT; i++)
    {
        inf_about_oper[i].hash = count_hash (inf_about_oper[i].name);
    }

    qsort (inf_about_oper, FUNC_COUNT, sizeof (inf_about_oper_t), &comp);

    fixup_t* fixup_array = (fixup_t*)calloc (fixup_array_size, sizeof (fixup_t));

    asm_context->fixup_context = {.fixup_table = fixup_array, .fixup_capacity = fixup_array_size};

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

int comp (const void* input_par_1, const void* input_par_2)
{
    long long par_1 = ((const inf_about_oper_t*) input_par_1)->hash;
    long long par_2 = ((const inf_about_oper_t*) input_par_2)->hash;

    return (par_1 > par_2) - (par_1 < par_2);
}

//--------------------------------------------------------------------------------

asm_error_t init_source_code_buffer (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context != NULL);

    asm_context->source_buffer.buffer_size = get_file_size (asm_context->read_file_name);

    if (asm_context->source_buffer.buffer_size == -1)
    {
        PRINTERR (ASM_ERR_OPEN_READ_FILE);

        return ASM_ERR_OPEN_READ_FILE;
    }

    asm_context->source_buffer.source_code_array = (char*)calloc (asm_context->source_buffer.buffer_size + 1, sizeof (char));

    if (asm_context->source_buffer.source_code_array == NULL)
    {
        PRINTERR (ASM_ERR_ALLOCATION);

        return ASM_ERR_ALLOCATION;
    }

    asm_context->source_buffer.source_code_array[asm_context->source_buffer.buffer_size] = '\0';

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

asm_error_t init_line_arr (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                                  != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array != NULL);

    asm_context->parsed_lines.line_array = (char**)calloc (asm_context->parsed_lines.num_lines, sizeof (char*));

    if (asm_context->parsed_lines.line_array == NULL)
    {
        PRINTERR (ASM_ERR_ALLOCATION);

        return ASM_ERR_ALLOCATION;
    }

    fill_line_array (asm_context);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

asm_error_t init_bytecode_buffer (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context != NULL);

    asm_context->bytecode_container.bytecode = (int* )calloc (asm_context->parsed_lines.num_lines * 2, sizeof (int));

    if (asm_context->bytecode_container.bytecode == NULL)
    {
        PRINTERR (ASM_ERR_ALLOCATION);
        
        return ASM_ERR_ALLOCATION;
    }

    memset (asm_context->labels, -1, labels_size * sizeof (int));

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

asm_error_t count_n_lines (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                                  != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array != NULL);

    int count = 0;
    char* tmp = asm_context->source_buffer.source_code_array;
    char* end = asm_context->source_buffer.source_code_array + asm_context->source_buffer.read_size;

    while (tmp < end)
    {
        tmp = strchr (tmp, '\n');

        if (tmp != NULL && tmp < end)
        {
            count++;
            tmp++;
        }
        else
        {
            break;
        }
    }

    asm_context->parsed_lines.num_lines = count + 1;

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

asm_error_t fill_line_array (asm_context_t* asm_context)
{
    DEBUG_ASSERT(asm_context                          != NULL);
    DEBUG_ASSERT(asm_context->parsed_lines.line_array != NULL);

    char* tmp = asm_context->source_buffer.source_code_array;
    
    for (int i = 0; i < asm_context->parsed_lines.num_lines; i++)
    {
        asm_context->parsed_lines.line_array[i] = tmp;

        tmp = strchr (tmp, '\n');

        if (tmp != NULL)
            tmp++;
        else
            break;
    }
    
    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

asm_error_t asm_destroy (asm_context_t* asm_context)
{
    DEBUG_ASSERT (asm_context                                  != NULL);
    DEBUG_ASSERT (asm_context->bytecode_container.bytecode     != NULL);
    DEBUG_ASSERT (asm_context->parsed_lines.line_array         != NULL);
    DEBUG_ASSERT (asm_context->source_buffer.source_code_array != NULL);

    free (asm_context->bytecode_container.bytecode    );
    free (asm_context->parsed_lines.line_array        );
    free (asm_context->source_buffer.source_code_array);
    free (asm_context->fixup_context.fixup_table      );

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------