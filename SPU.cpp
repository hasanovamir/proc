#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
    stack_t      stk  = {};
    parameters_t data = {};

    RETURN_IF_STACK_ERR (stack_init          (&stk, 100)     );
    RETURN_IF_SPU_ERR   (allocation_com_arr  (&data, argv[1]));
    RETURN_IF_SPU_ERR   (read_commands       (&data, argv[1]));
    RETURN_IF_STACK_ERR (use_stack_func      (&data, &stk)   );
    RETURN_IF_STACK_ERR (stack_destroy       (&stk)          );
    RETURN_IF_SPU_ERR   (spu_destroy_com_arr (&data)         ); 
}

//--------------------------------------------------------------------------------

spu_err_t allocation_com_arr (parameters_t* data, const char* title)
{
    DEBUG_ASSERT (data  != NULL);
    DEBUG_ASSERT (title != NULL);

    RETURN_IF_SPU_ERR (get_file_size (data, title));
    data->num_el /= 4;

    data->com_arr = (int*)calloc (data->num_el, sizeof (int));
    if (data->com_arr == NULL)
    {
        return SPU_ALLOCATION_ERR;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t read_commands (parameters_t* data, const char* title)
{
    DEBUG_ASSERT (data          != NULL);
    DEBUG_ASSERT (data->com_arr != NULL);
    DEBUG_ASSERT (title         != NULL);

    FILE* file = fopen (title, "r");
    if (file == NULL)
    {
        return SPU_OPEN_FILE_ERR;
    }

    data->num_el = fread (data->com_arr, sizeof (int), data->num_el, file);

    fclose (file);

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

stack_err_t use_stack_func (parameters_t* data, stack_t* stk)
{
    STACK_OK (stk);

    DEBUG_ASSERT (data          != NULL);
    DEBUG_ASSERT (data->com_arr != NULL);

    int pos = 1;
    
    while (1)
    {
        int a = 0, b  = 0;

        if (pos >= data->num_el)
        {
            fprintf (stderr, "NO HALT IN COMMANDS");
            return STACK_UNKNOWN_ERR;
        }

        switch ((operation_t)(data->com_arr[pos]))
        {
            case PUSH:
                RETURN_IF_STACK_ERR (stack_push (stk, data->com_arr[++pos]));
                data->num_compl_com++;
                pos++;
                break;
            case OUT:
                RETURN_IF_STACK_ERR (stack_pop (stk, &a));
                printf ("%d\n", a);
                data->num_compl_com++;
                pos++;
                break;
            case ADD:
                RETURN_IF_STACK_ERR (stack_pop  (stk, &a)   );
                RETURN_IF_STACK_ERR (stack_pop  (stk, &b)   );
                RETURN_IF_STACK_ERR (stack_push (stk, a + b));
                data->num_compl_com++;
                pos++;
                break;
            case SUB:
                RETURN_IF_STACK_ERR (stack_pop  (stk, &a)   );
                RETURN_IF_STACK_ERR (stack_pop  (stk, &b)   );
                RETURN_IF_STACK_ERR (stack_push (stk, b - a));
                data->num_compl_com++;
                pos++;
                break;
            case DIV:
                RETURN_IF_STACK_ERR (stack_pop  (stk, &a)   );
                RETURN_IF_STACK_ERR (stack_pop  (stk, &b)   );
                RETURN_IF_STACK_ERR (stack_push (stk, b / a));
                data->num_compl_com++;
                pos++;
                break;
            case MUL:
                RETURN_IF_STACK_ERR (stack_pop  (stk, &a)   );
                RETURN_IF_STACK_ERR (stack_pop  (stk, &b)   );
                RETURN_IF_STACK_ERR (stack_push (stk, a * b));
                data->num_compl_com++;
                pos++;
                break;
            case SQRT:
                RETURN_IF_STACK_ERR (stack_pop  (stk, &a));
                RETURN_IF_STACK_ERR (stack_push (stk, (stack_val_t)sqrt (a)));
                data->num_compl_com++;
                pos++;
                break;
            case HLT:
                if (data->num_compl_com != data->com_arr[0] - 1)
                    printf ("WARNING, not all commands are completed");
                return STACK_SUCCESS;
            default:
                return STACK_SCAN_ERR;
        }
    }
}

//--------------------------------------------------------------------------------

const char* error_spu_code_to_string (spu_err_t status)
{
    switch (status)
    {
        ERRCASE (SPU_SUCCESS       );
        ERRCASE (SPU_ALLOCATION_ERR);
        ERRCASE (SPU_OPEN_FILE_ERR );
        default:
            return "SPU_SUCCESS";
    }
}

//--------------------------------------------------------------------------------

spu_err_t spu_destroy_com_arr (parameters_t* data)
{
    DEBUG_ASSERT (data          != NULL);
    DEBUG_ASSERT (data->com_arr != NULL);

    free (data->com_arr);
    data->num_el = 0;

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

spu_err_t get_file_size (parameters_t* data, const char* title)
{
    DEBUG_ASSERT (data  != NULL);
    DEBUG_ASSERT (title != NULL);

    struct stat statistic = {};
    stat (title, &statistic);

    data->num_el = statistic.st_size;
    if (data->num_el == 0)
    {
        return SPU_OPEN_FILE_ERR;
    }

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------