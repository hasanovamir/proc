#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
    stack_t stk = {};
    spu_context_t spu_context = {};
    int reg[16] = {};

    if (spu_init (&spu_context, argv[1], &stk))
    {
        spu_destroy_command_array (&spu_context, &stk);
        return EXIT_FAILURE;
    }

    if (read_commands (&spu_context, argv[1]))
    {
        spu_destroy_command_array (&spu_context, &stk);
        return EXIT_FAILURE;
    }

    if (spu_execute_instructions (&spu_context, &stk, reg))
    {
        spu_destroy_command_array (&spu_context, &stk);
        return EXIT_FAILURE;
    }

    spu_destroy_command_array (&spu_context, &stk); 

    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------