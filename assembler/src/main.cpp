#include <cstring>
#include "command.h"
#include "AsmFunctions.h"
#include "FileInput.h"

int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf(RED "You don't entered names of files\n" WHITE);
        abort();
    } else if (argc == 2) {
        printf(RED "You entered name of file %s\n" WHITE, argv[1]);
        abort();
    } else if (argc > 4){
        printf(RED "You entered too many names of files\n" WHITE);
        abort();
    }
    
    RunAssembler(argv[2], argv[1]);
    disassembly("disasmled.txt", argv[2]);
    
    return Error;
}