#include "assembler.h"

//--------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
    asm_context_t asm_context = {};

    if (asm_init(&asm_context, argc, argv))
    {
        asm_destroy (&asm_context);

        return EXIT_FAILURE;
    }

    if (init_source_code_buffer (&asm_context))
    {
        asm_destroy (&asm_context);

        return EXIT_FAILURE;
    }

    if (read_source_code (&asm_context))
    {
        asm_destroy (&asm_context);

        return EXIT_FAILURE;
    }

    if (init_line_arr (&asm_context))
    {
        asm_destroy (&asm_context);

        return EXIT_FAILURE;
    }

    if (init_bytecode_buffer (&asm_context))
    {
        asm_destroy (&asm_context);

        return EXIT_FAILURE;
    }

    assemble (&asm_context);

    if (write_bytecode (&asm_context))
    {
        asm_destroy (&asm_context);

        return EXIT_FAILURE;
    }

    printf ("%s %d", asm_context.parsed_lines.line_array[asm_context.parsed_lines.num_lines], asm_context.parsed_lines.num_lines);

    asm_destroy (&asm_context);

    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------