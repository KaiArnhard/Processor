#include "command.h"
#include "FileInput.h"
#include "types.h"

int main() {
    char AsmName[100] =  {};
    
    printf("Enter full path to assembler file\n");
    scanf("%s", AsmName);
    
    assembly("../../cm.txt", AsmName);
    disassembly("disasm.txt", "../../cm.txt");
}