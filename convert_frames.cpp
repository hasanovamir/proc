#include "general_io_file_func.h"

//--------------------------------------------------------------------------------

int func_hz  (void);

//--------------------------------------------------------------------------------

int FileSize = 3493;

//--------------------------------------------------------------------------------

int main (void)
{
    func_hz ();   
}

//--------------------------------------------------------------------------------

int func_hz (void){

    FILE* writeable_file = fopen ("translated.txt", "a");

    if (writeable_file == NULL)
    {
        fprintf (stderr, "PIZDEC BLYAT FILE NE OTKRILSYA KOTORI PISHEM\n");

        return 1;
    }
    char* PreviousFrame = (char*) calloc (FileSize, sizeof (char));

    char* Frame         = (char*) calloc (FileSize, sizeof (char));

    for (int i = 1; i < 6572; i++){
        char FileName[100] = "";

        snprintf (FileName, 100, "bad-apple-ascii-main/frames-ascii/out%04d.jpg.txt", i);

        FILE* readable_file = fopen (FileName, "r");

        if (readable_file == NULL)
        {
            fprintf (stderr, "PIZDEC BLYAT FILE NE OTKRILSYA KOTORI CHITAEM\n");

            return 1;
        }

        if (readable_file == NULL)
        {
            fprintf (stderr, "EBAT PC AHUEL PAMYAT NE DAL DLYA BUFFER\n\n");

            return 1;
        }

        FileSize = fread (Frame, sizeof (char), FileSize, readable_file);

        fclose (readable_file);

        char* source_code = (char*) calloc (60 * FileSize, sizeof (char));

        if (source_code == NULL)
        {
            fprintf (stderr, "EBAT PC AHUEL PAMYAT NE DAL DLYA SOURCECOEDE\n\n");

            return 1;
        }

        printf ("%d\n", i);

        for (int j = 0, pos = 0; j < FileSize; j++){
            if (Frame[j] == '\n')
            {
                continue;
            }

            if (Frame[j] != PreviousFrame[j])
            {
                char num[10] = "";

                snprintf (num, 10, "PUSH %d\n", Frame[j]);

                strcat (source_code, num);

                snprintf (num, 10, "%d\n", pos);

                strcat (source_code, "PUSH ");

                strcat (source_code, num);

                strcat (source_code, "POPR REG1\n");

                strcat (source_code, "POPM [REG1]\n");
            }

            pos++;
        }

        memcpy (PreviousFrame, Frame, FileSize);

        strcat (source_code, "DRAW\n&");

        char* ampersand_pos = strchr(source_code, '&');

        int count;

        if (ampersand_pos != NULL)
        {
            count = ampersand_pos - source_code;
        }
        else
        {
            count = strlen(source_code);
        }

        fprintf (stderr, "file %d\n", i);

        fwrite (source_code, sizeof (char), count, writeable_file);

        free (source_code);
    }

    free (Frame        );
    free (PreviousFrame);

    fclose (writeable_file);

    return 0;
}

//--------------------------------------------------------------------------------