#include "asm.h"
#include "command.h"

int disassembly(const char* DisAsmName, const char* CmName) {
    FILE* PtrToAsm = fopen(DisAsmName, "w");
    FILE* PtrToCm  = fopen(CmName,     "rb");
    
    int tmp = __INT_MAX__;
    size_t inf[3] = {};

    fread(inf, sizeof(size_t), 3, PtrToCm);

    for (size_t i = 0; i < 3; i++) {
        fprintf(PtrToCm, "%d\n", inf[i]);
    }
    
    int* command = (int*) calloc(inf[2], sizeof(int));
    fread(command, sizeof(int), inf[2], PtrToCm);

    for (size_t counter = 0; counter < inf[2]; counter++) {
        switch (command[counter] & BITCOMM) {
        case (CMD_HLT & BITCOMM):
            fprintf(PtrToAsm, "HLT\n");
            break;
        case CMD_PUSH:
            switch (command[counter] & IDENTIF)
            {
            case immed:
                fprintf(PtrToAsm, "PUSH Number ");
                fprintf(PtrToAsm, "%d\n", command[++counter]);
                break;
            case regis:
                fprintf(PtrToAsm, "PUSH Register ");
                fprintf(PtrToAsm, "%d\n", command[++counter]);
                break;
            default:
                break;
            }
            break;
        case CMD_IN:
            fprintf(PtrToAsm, "IN\n");
            break;
        case CMD_POP:
            fprintf(PtrToAsm, "POP\n");
            tmp = command[++counter];
            break;
        case CMD_ADD:
            fprintf(PtrToAsm, "ADD\n");
            break;
        case CMD_SUB:
            fprintf(PtrToAsm, "SUB\n");
            break;
        case CMD_DIV:
            fprintf(PtrToAsm, "DIV\n");
            break;
        case CMD_MUL:
            fprintf(PtrToAsm, "MUL\n");
            break;
        case CMD_SQRT:
            fprintf(PtrToAsm, "SQRT\n");
            break;
        case CMD_SIN:
            fprintf(PtrToAsm, "SIN\n");
            break;
        case CMD_COS:
            fprintf(PtrToAsm, "COS\n");
            break;
        case CMD_OUT:
            fprintf(PtrToAsm, "OUT\n");
            break;
        case CMD_JMP:
            fprintf(PtrToAsm, "JMP\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JA:
            fprintf(PtrToAsm, "JA\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JAE:
            fprintf(PtrToAsm, "JAE\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JB:
            fprintf(PtrToAsm, "JB\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JBE:
            fprintf(PtrToAsm, "JBE\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JE:
            fprintf(PtrToAsm, "JE\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JNE:
            fprintf(PtrToAsm, "JNE\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_CALL:
            fprintf(PtrToAsm, "JE\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;   
        case CMD_RET:
            fprintf(PtrToAsm, "CALL\n");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;     
        default:
            MyAssert(1 && "Wrong instruction",(char*) command + counter);
            Error = -1;
            return Error;
            break;
        }    
    }

    free(command);
    fclose(PtrToAsm);
    fclose(PtrToCm);

    return 0;
}

void MyAssertionFailed(const char* condition, const char* command, const char* file, const char* function, const int line) {
    printf(RED "Assertion failed on file %s on function %s:%d\n", file, function, line);
    printf("Wrong condition: %s\n", condition);
    printf("Intered %s\n", command);
    printf(WHITE);
}