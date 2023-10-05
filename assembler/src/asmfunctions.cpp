#include <cstdio>
#include "command.h"
#include "FileInput.h"
#include <cstring>

int assembly(const char* fname1, const char* fname2) {
    FILE* PtrToCm = fopen(fname1, "w");
    String* PtrToStr = nullptr;
    Lengths length = {};
    char* buffer = InputBuffer(fname2, &length);
    
    PtrToStr = (String*) calloc(length.NumberOfLines, sizeof(String));
    
    InputPtrToBuffer(PtrToStr, &length, buffer);
    comparator(PtrToStr, length.NumberOfLines, PtrToCm);
    
    free(PtrToStr);   
    fclose(PtrToCm);
    return 0;
}

void comparator(String* PtrToStrs, size_t NumbOfLines, FILE* PtrToCm) {
    
    #define COMMAND_COMPARE(command, buffer, command_number)  \
    if (!strcmp(buffer, command)) {                           \
        fprintf(PtrToCm, "%d ", command_number);              \
        if (value != __INT_MAX__) {                           \
            fprintf(PtrToCm, "%d\n", value);                  \
        } else {                                              \
            fprintf(PtrToCm, "\n");                           \
        }                                                     \
    }           

    char buff[100] = {};

    fprintf(PtrToCm, "%d\n", NumbOfLines);
    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        int value = __INT_MAX__;
        sscanf(PtrToStrs[counter].ptrtostr, "%s%d", buff, &value);
        
        #include "command_compare.h"
        
        else COMMAND_COMPARE("IN", buff, STACK_IN)
        else COMMAND_COMPARE("POP", buff, STACK_POP)
        else COMMAND_COMPARE("ADD", buff, ADD)
        else COMMAND_COMPARE("SUB", buff, SUB)
        else COMMAND_COMPARE("DIV", buff, DIV)
        else COMMAND_COMPARE("SQRT", buff, SQRT)
        else COMMAND_COMPARE("SIN", buff, SIN)
        else COMMAND_COMPARE("COS", buff, COS)
        else COMMAND_COMPARE("OUT", buff, OUT)
        else COMMAND_COMPARE("HLT", buff, HLT)
        else {
            printf("\033[0;31m");
            printf("Syntax Error\n");
            printf("Entered command is %s\n", buff);
        }
    }
}

int disassembly(char* DisAsmName, char* CmName) {
    FILE* PtrToAsm = fopen(DisAsmName, "w");
    FILE* PtrToCm  = fopen(CmName,     "r");
    
    int command = __INT_MAX__;
    int tmp = __INT_MAX__;
    size_t NumbOfLines = 0;
    
    fscanf(PtrToCm, "%d", &NumbOfLines);

    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        fscanf(PtrToCm, "%d", &command);    
        switch (command) {
        case HLT:
            fprintf(PtrToAsm, "HLT\n");
            break;
        case STACK_PUSH:
            fprintf(PtrToAsm, "PUSH ");
            fscanf(PtrToCm, "%d", &tmp);
            fprintf(PtrToAsm, "%d\n", tmp);
            break;
        case STACK_IN:
            fprintf(PtrToAsm, "IN\n");
            break;
        case STACK_POP:
            fprintf(PtrToAsm, "POP\n");
            break;
        case ADD:
            fprintf(PtrToAsm, "ADD\n");
            break;
        case SUB:
            fprintf(PtrToAsm, "SUB\n");
            break;
        case DIV:
            fprintf(PtrToAsm, "DIV\n");
            break;
        case MUL:
            fprintf(PtrToAsm, "MUL\n");
            break;
        case SQRT:
            fprintf(PtrToAsm, "SQRT\n");
            break;
        case SIN:
            fprintf(PtrToAsm, "SIN\n");
            break;
        case COS:
            fprintf(PtrToAsm, "COS\n");
            break;
        case OUT:
            fprintf(PtrToAsm, "OUT\n");
            break;
        default:
            abort();
        break;
        }    
    }

    fclose(PtrToAsm);
    fclose(PtrToCm);

    return 0;
}