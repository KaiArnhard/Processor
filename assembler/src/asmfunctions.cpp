#include <cstdio>
#include "../include/command.h"
#include "../include/FileInput.h"
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
    char buff[100] = {};
    int value = __INT_MAX__;

    fprintf(PtrToCm, "%d\n", NumbOfLines);
    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        sscanf(PtrToStrs[counter].ptrtostr, "%s", buff);
        if (!strcmp(buff, "PUSH")) {
            fprintf(PtrToCm, "%d ", STACK_PUSH);
            sscanf(PtrToStrs[counter].ptrtostr + strlen("PUSH"), "%d", &value);
            fprintf(PtrToCm, "%d\n", value);
        } else if (!strcmp(buff, "IN")) {
            fprintf(PtrToCm, "%d\n", STACK_IN);
        } else if (!strcmp(buff, "ADD")) {
            fprintf(PtrToCm, "%d\n", ADD);
        } else if (!strcmp(buff, "SUB")) {
            fprintf(PtrToCm, "%d\n", SUB);
        } else if (!strcmp(buff, "DIV")) {
            fprintf(PtrToCm, "%d\n", DIV);
        } else if (!strcmp(buff, "MUL")) {
            fprintf(PtrToCm, "%d\n", MUL);
        } else if (!strcmp(buff, "SQRT")) {
            fprintf(PtrToCm, "%d\n", SQRT);
        } else if (!strcmp(buff, "SIN")) {
            fprintf(PtrToCm, "%d\n", SIN);
        } else if (!strcmp(buff, "COS")) {
            fprintf(PtrToCm, "%d\n", COS);
        } else if (!strcmp(buff, "OUT")) {
            fprintf(PtrToCm, "%d\n", OUT);
        } else if (!strcmp(buff, "HLT")) {
            fprintf(PtrToCm, "%d", HLT);
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