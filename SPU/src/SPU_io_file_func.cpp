#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

spu_err_t read_commands (spu_context_t* spu_context, const char* file_name)
{
    DEBUG_ASSERT (spu_context          != NULL);
    DEBUG_ASSERT (spu_context->command_array != NULL);
    DEBUG_ASSERT (file_name            != NULL);

    FILE* file = fopen (file_name, "rb");
    if (file == NULL)
    {
        return SPU_OPEN_FILE_ERR;
    }

    int read_size = fread (spu_context->command_array, sizeof (int), spu_context->command_array_size, file);

    if (spu_context->command_array_size != read_size)
    {
        fprintf(stderr, "SPU_FREAD_ERR in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return SPU_FREAD_ERR;
    }

    fclose (file);

    return SPU_SUCCESS;
}

//--------------------------------------------------------------------------------

int get_file_size (const char* file_name)
{
    DEBUG_ASSERT (file_name != NULL);

    struct stat statistic = {};
    stat (file_name, &statistic);

    int file_size = statistic.st_size;
    
    if (file_size == 0)
    {
        fprintf(stderr, "ERR_get_file_size in %s:%d func:%s\n",
            __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return -1;
    }

    return file_size;
}

//--------------------------------------------------------------------------------