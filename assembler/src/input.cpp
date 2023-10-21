#include <cstring>
#include "command.h"
#include "FileInput.h"

char *ReadFileToBuffer(const char *FileName, Lengths *Length)
{

    struct stat statistic = {};
    stat(FileName, &statistic);

    Length->LengthOfBuffer = statistic.st_size;
    char *Buffer = (char*) calloc(Length->LengthOfBuffer, sizeof(char));

    FILE *fp = fopen (FileName, "r");
    assert(fp != 0);
    assert(fread(Buffer, sizeof(char), statistic.st_size, fp) != 0);
    fclose(fp);
    
    for (size_t counter = 0; counter < Length->LengthOfBuffer; counter++) {
        Buffer[counter] = toupper(Buffer[counter]);
    }

    Length->NumberOfLines++;
    for (size_t counter = 0; (counter + 1) < Length->LengthOfBuffer; counter++) {
        if (Buffer[counter] == '\n') {
            Buffer[counter] = '\0';
            Length->NumberOfLines++;
        }
    }
    return Buffer;
}

void InputPtrToBuffer (String *PtrToLine, Lengths *Length, char *Buffer) {
    size_t counter  = 0;
    size_t counter1 = 0;

    for (; counter < Length->LengthOfBuffer && counter1 < Length->NumberOfLines; counter++) {
        if (Buffer[counter - 1] == '\0') {
            PtrToLine[counter1].ptrtostr = Buffer + counter;
            PtrToLine[counter1].lengthofstr = strlen(PtrToLine[counter1].ptrtostr);
            if (PtrToLine[counter1].lengthofstr > 5) {
                Length->NumberOfCommands += 2;
            } else {
                Length->NumberOfCommands++;
            }
            counter1++;
        }
    }
    for (size_t count = 0; count < counter1; count++) {
        char* tmp = strchr(PtrToLine[count].ptrtostr, ';');
        if (tmp != nullptr) {
            tmp[0] = '\0';    
        }
    }
    Length->NumberOfLines = counter1;
}
