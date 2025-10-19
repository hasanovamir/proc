#include "stack.h"
#include "SPU.h"

//--------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
    spu_context_t spu_context = {};

    if (spu_init (&spu_context, argc, argv))
    {
        spu_destroy (&spu_context);
        return EXIT_FAILURE;
    }

    if (read_bytecode (&spu_context))
    {
        spu_destroy (&spu_context);
        return EXIT_FAILURE;
    }

    if (spu_execute_instructions (&spu_context))
    {
        spu_destroy (&spu_context);
        return EXIT_FAILURE;
    }

    spu_destroy (&spu_context); 

    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------