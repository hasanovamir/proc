#include "assembler.h"

//--------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
    asm_context_t asm_context = {};

    if (allocate_el_arr (&asm_context, argv[1]))
    {
        destroy_arrays (&asm_context);

        return EXIT_FAILURE;
    }

    if (read_commands (&asm_context, argv[1]))
    {
        destroy_arrays (&asm_context);

        return EXIT_FAILURE;
    }

    if (allocate_line_arr (&asm_context))
    {
        destroy_arrays (&asm_context);

        return EXIT_FAILURE;
    }

    if (allocate_com_arr (&asm_context))
    {
        destroy_arrays (&asm_context);

        return EXIT_FAILURE;
    }

    compile (&asm_context);

    if (write_commands (&asm_context, argv[2]))
    {
        destroy_arrays (&asm_context);

        return EXIT_FAILURE;
    }

    destroy_arrays (&asm_context);

    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------