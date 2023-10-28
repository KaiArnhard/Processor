#include "ProcFunctions.h"
#include "command.h"

int main(int argc, char* argv[]) {
    SPU_t proc = {};
    if (argc == 1) {
        printf(RED "You don't entered name of file\n");
    } else if (argc > 2) {
        printf(RED "You entered too many names of files\n");
    }
    
    SPUCtor(&proc, argv[1]);

    VirtualMachine(&proc);
    
    SPUDtor(&proc);
    return 0;
}