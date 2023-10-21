#include "proc.h"
#include "command.h"

int main(int argc, char* argv[]) {
    SPU_t proc = {};
    if (argc == 1) {
        printf(RED "You don't entered name of file\n");
    } else if (argc > 2) {
        printf(RED "You entered too many names of files\n");
    }
    
    FILE* PtrToCM = fopen(argv[1], "rb");

    SPUCtor(&proc, PtrToCM);

    VirtualMachine(&proc, PtrToCM);
    
    SPUDtor(&proc);
    return Error;
}