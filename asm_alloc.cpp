#include "assembler.h"

//--------------------------------------------------------------------------------

error_t allocation_el_arr (parametrs_t* data, const char* title)
{
    DEBUG_ASSERT (data != NULL );
    DEBUG_ASSERT (title != NULL);

    struct stat statistic = {};
    stat (title, &statistic);

    data->num_el = statistic.st_size;

    data->el_array = (char*)calloc (data->num_el + 1, sizeof (char));

    if (data->el_array == NULL)
    {
        return ASM_ERR_ALLOCATION;
    }

    data->el_array[data->num_el] = '\0';

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t allocation_line_com_arr (parametrs_t* data)
{
    DEBUG_ASSERT (data != NULL          );
    DEBUG_ASSERT (data->el_array != NULL);
    
    RETURN_IF_ERR (count_n_lines (data) );

    data->com_array  = (char* )calloc (data->num_lines * 20, sizeof (char));
    data->line_array = (char**)calloc (data->num_lines, sizeof (char*)    );
    if (data->com_array == NULL || data->line_array == NULL)
    {
        return ASM_ERR_ALLOCATION;
    }

    RETURN_IF_ERR (filling_line_array (data));

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t count_n_lines (parametrs_t* data)
{
    DEBUG_ASSERT (data != NULL          );
    DEBUG_ASSERT (data->el_array != NULL);

    int i = 0;
    char* tmp = data->el_array;

    while (tmp != NULL)
    {
        tmp = strchr (tmp, '\n');

        if (tmp != NULL)
        {
            i++;
            tmp++;
        }
    }

    data->num_lines = i + 1;

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t filling_line_array (parametrs_t* data)
{
    DEBUG_ASSERT (data != 0            );
    DEBUG_ASSERT (data->line_array != 0);

    char* tmp = data->el_array;

    for (int i = 0; i < data->num_lines; i++)
    {
        (data->line_array)[i]  = tmp;
        tmp = strchr (tmp, '\n') + 1;
    }

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

const char* error_code_to_string (error_t status)
{
    switch (status)
    {
        ERRCASE (ASM_ERR_ALLOCATION     );
        ERRCASE (ASM_ERR_OPEN_READ_FILE );
        ERRCASE (ASM_ERR_FREAD          );
        ERRCASE (ASM_ERR_LINE_ARR_FILL  );
        ERRCASE (ASM_ERR_INVAID_SCAN_SYM);
        ERRCASE (ASM_ERR_OPEN_WRITE_FILE);
        ERRCASE (ASM_SUCCESS            );
        default:
            return "ASM_SUCCESS";
    }
}

//--------------------------------------------------------------------------------