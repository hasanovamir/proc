#include "assembler.h"

//--------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
    context_t context = {};
    int labels[10]    = {};

    if (allocate_el_arr (&context, argv[1]))
    {
        destroy_arrays (&context);

        return EXIT_FAILURE;
    }

    if (read_commands (&context, argv[1]))
    {
        destroy_arrays (&context);

        return EXIT_FAILURE;
    }

    if (allocate_line_arr (&context))
    {
        destroy_arrays (&context);

        return EXIT_FAILURE;
    }

    if (allocate_com_arr (&context))
    {
        destroy_arrays (&context);

        return EXIT_FAILURE;
    }

    if (compile (&context, labels))
    {
        destroy_arrays (&context);

        return EXIT_FAILURE;
    }

    if (write_commands (&context, argv[2]))
    {
        destroy_arrays (&context);

        return EXIT_FAILURE;
    }

    destroy_arrays (&context);

    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------