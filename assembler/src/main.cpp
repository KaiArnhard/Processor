#include "../include/command.h"
#include "../include/FileInput.h"
#include "../include/types.h"

int main() {
    char AsmName[100] =  {};
    
    printf("Enter full path to assembler file\n");
    scanf("%s", AsmName);
    
    assembly("../../cm.txt", AsmName);
}