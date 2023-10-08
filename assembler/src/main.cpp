#include "command.h"
#include "FileInput.h"

int main() {
    char AsmName[100] =  {};
    
    printf("Enter full path to assembler file\n");
    scanf("%s", AsmName);
    
    assembly(PathToCm1, AsmName);
}