#include "assembler.h"

//--------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
    parameters_t data = {};

    RETURN_IF_ERR (allocation_el_arr   (&data, argv[1]));
    RETURN_IF_ERR (read_commands       (&data, argv[1]));
    RETURN_IF_ERR (allocation_line_arr (&data)         );
    RETURN_IF_ERR (allocation_com_arr  (&data)         );
    RETURN_IF_ERR (translator          (&data)         );
    RETURN_IF_ERR (write_commands      (&data, argv[2]));
    RETURN_IF_ERR (destroy_arrays      (&data)         );

    return 0;
}

//--------------------------------------------------------------------------------

error_t read_commands (parameters_t* data, const char* title)
{
    DEBUG_ASSERT (data           != NULL);
    DEBUG_ASSERT (data->el_array != NULL);
    DEBUG_ASSERT (title          != NULL);

    FILE* file = fopen (title, "r");

    if (file == NULL)
    {
        return ASM_ERR_OPEN_READ_FILE;
    }

    data->num_el = (int)fread (data->el_array, sizeof (char), data->num_el, file);

    if (data->num_el == 0)
    {
        return ASM_ERR_FREAD;
    }

    fclose (file);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t translator (parameters_t* data)
{
    DEBUG_ASSERT (data             != NULL);
    DEBUG_ASSERT (data->line_array != NULL);
    DEBUG_ASSERT (data->com_array  != NULL);

    int pos_el_arr = 0, pos_com_arr = 1;

    for (int i = 0; i < data->num_lines; i++)
    {
        char tmp[10] = "";
        
        sscanf (data->line_array[i], "%s", tmp);

        if (strcmp (tmp, "PUSH") == 0)
        {
            int value = 0;
            sscanf (data->line_array[i] + 4, "%d", &value);

            data->com_array[pos_com_arr++] = 1;
            data->com_array[pos_com_arr++] = value;

            pos_el_arr += 2;

            int    count  = 0;
            while (value != 0)
            {
                count++;
                value /= 10;
            }

            data->num_com_arr_el += 2;
            data->num_com++;

            pos_el_arr += count;
            continue;
        }
        else if (strcmp (tmp, "OUT") == 0)
        {
            data->com_array[pos_com_arr++] = 2;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "ADD") == 0)
        {
            data->com_array[pos_com_arr++] = 3;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "SUB") == 0)
        {
            data->com_array[pos_com_arr++] = 4;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "MUL") == 0)
        {
            data->com_array[pos_com_arr++] = 5;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "DIV") == 0)
        {
            data->com_array[pos_com_arr++] = 6;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "SQRT") == 0)
        {
            data->com_array[pos_com_arr++] = 7;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "HLT") == 0)
        {
            data->com_array[pos_com_arr++] = 8;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "IN") == 0)
        {
            data->com_array[pos_com_arr++] = 9;
            data->num_com_arr_el++;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "POPR") == 0)
        {
            int a = 0;
            sscanf (data->line_array[i] + 4, "%s", tmp);
            a = convert_name_of_reg_to_num (tmp);
            if (a == -1)
            {
                return ASM_ERR_UNKNOWN_REG_NAME;
            }
            data->com_array[pos_com_arr++] = 42;
            data->com_array[pos_com_arr++] = a;
            data->num_com_arr_el += 2;
            data->num_com++;
            pos_el_arr += 2;
        }
        else if (strcmp (tmp, "PUSHR") == 0)
        {
            int a = 0;
            sscanf (data->line_array[i] + 5, "%s", tmp);
            a = convert_name_of_reg_to_num (tmp);
            if (a == -1)
            {
                return ASM_ERR_UNKNOWN_REG_NAME;
            }
            data->com_array[pos_com_arr++] = 33;
            data->com_array[pos_com_arr++] = a;
            data->num_com_arr_el += 2;
            data->num_com++;
            pos_el_arr += 2;
        }
        else
        {
            printf ("UNKNOWN COMMAND\n");

            return ASM_ERR_INVALID_SCAN_SYM;
        }
    }

    data->com_array[0] = data->num_com;
    data->num_com_arr_el++;

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t write_commands (parameters_t* data, const char* title)
{
    DEBUG_ASSERT (data != 0 );
    DEBUG_ASSERT (title != 0);
    
    FILE* file = fopen (title, "w");

    if (file == NULL)
    {
        return ASM_ERR_OPEN_WRITE_FILE;
    }

    fwrite (data->com_array, sizeof (int), data->num_com_arr_el, file);
    
    fclose (file);

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

error_t destroy_arrays (parameters_t* data)
{
    DEBUG_ASSERT (data             != NULL);
    DEBUG_ASSERT (data->el_array   != NULL);
    DEBUG_ASSERT (data->line_array != NULL);
    DEBUG_ASSERT (data->com_array  != NULL);

    free (data->com_array );
    free (data->line_array);
    free (data->el_array  );

    return ASM_SUCCESS;
}

//--------------------------------------------------------------------------------

int convert_name_of_reg_to_num (char* name)
{
    DEBUG_ASSERT (name != NULL);

    if (strcmp (name, "REG1") == 0)
        return 1;
    else if (strcmp (name, "REG2") == 0)
        return 2;
    else if (strcmp (name, "REG3") == 0)
        return 3;
    else if (strcmp (name, "REG4") == 0)
        return 4;
    else if (strcmp (name, "REG5") == 0)
        return 5;
    else if (strcmp (name, "REG6") == 0)
        return 6;
    else if (strcmp (name, "REG7") == 0)
        return 7;
    else if (strcmp (name, "REG8") == 0)
        return 8;
    else if (strcmp (name, "REG9") == 0)
        return 9;
    else if (strcmp (name, "REG10") == 0)
        return 10;
    else if (strcmp (name, "REG11") == 0)
        return 11;
    else if (strcmp (name, "REG12") == 0)
        return 12;
    else if (strcmp (name, "REG13") == 0)
        return 13;
    else if (strcmp (name, "REG14") == 0)
        return 14;
    else if (strcmp (name, "REG15") == 0)
        return 15;
    else if (strcmp (name, "REG16") == 0)
        return 16;

    return -1;
}