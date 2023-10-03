#include "../include/Mystring.h"
#include <cstring>
#include "../include/io.h"

char *InputBuffer (const char *FileName, Lengths *Length)
{

    struct stat statistic = {};
    stat(FileName, &statistic);

    Length->LengthOfBuffer = statistic.st_size;
    char *Buffer = (char*) calloc(Length->LengthOfBuffer, sizeof(char));

    FILE *fp = fopen (FileName, "r");
    assert(fp != 0);
    assert(fread(Buffer, sizeof(char), statistic.st_size, fp) != 0);
    fclose(fp);

    Length->NumberOfLines++;
    for (int counter = 0; (counter + 1) < Length->LengthOfBuffer; counter++)
    {
        if (Buffer[counter] == '\n')
        {
            Buffer[counter] = '\0';
            Length->NumberOfLines++;
        }
    }
    return Buffer;
}

void InputPtrToBuffer (String *PtrToLine, Lengths *Length, char *Buffer)
{
    int counter  = 0;
    int counter1 = 0;

    for (; counter < Length->LengthOfBuffer && counter1 < Length->NumberOfLines; counter++)
    {
        if (Buffer[counter - 1] == '\0')
        {
            while(!isalpha(Buffer[counter]))
            {
                counter++;
            }
            (PtrToLine + counter1)->ptrtostr = Buffer + counter;
            (PtrToLine + counter1)->lengthofstr = strlen(PtrToLine[counter1].ptrtostr);
            counter1++;
        }
    }
    Length->NumberOfLines = counter1;
}

