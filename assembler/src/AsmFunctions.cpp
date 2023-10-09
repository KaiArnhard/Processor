#include <cstdio>
#include "command.h"
#include "FileInput.h"
#include <cstring>

#define COMMAND_COMPARE(StrCommand, buffer, command_number)                                                            \
    if (!strcmp(buffer, StrCommand)) {                                                                                 \
        command[NumbOfComs] = command_number;                                                                          \
        NumbOfComs++;                                                                                                  \
        if (value != __INT_MAX__) {                                                                                    \
            command[NumbOfComs - 1] += immed;                                                                          \
            command[NumbOfComs] = value;                                                                               \
            NumbOfComs++;                                                                                              \
        } else if ((dbg = sscanf(PtrToStr[counter].ptrtostr + strlen(StrCommand), "%s", reg)) != 0 && dbg != EOF) {    \
            if (reg[0] != 'R' || reg[2] != 'X' || (reg[1] - 'A') > 3) {                                                \
                command[NumbOfComs - 1] = REG_ERR;                                                                     \
            } else {                                                                                                   \
                command[NumbOfComs - 1] += regis;                                                                      \
                command[NumbOfComs] = reg[1] - 'A';                                                                    \
                NumbOfComs++;                                                                                          \
            }                                                                                                          \
        }                                                                                                              \
    }           

int assembly(const char* PathToCm, const char* PathToAsm) {
    FILE* fp = fopen(PathToCm, "wb");
    assert(fp != nullptr);

    String* PtrToStr = nullptr;
    Lengths length = {};

    char* buffer = InputBuffer(PathToAsm, &length);
    
    PtrToStr = (String*) calloc(length.NumberOfLines, sizeof(String));
    
    InputPtrToBuffer(PtrToStr, &length, buffer);
    
    #if defined(LINUX)
        key_t key = ftok(PathToCm, proj_id);
        size_t size = (4 + length.NumberOfCommands) * sizeof(int);
        int shm_id = shmget(key, size, 0666 | IPC_CREAT |IPC_EXCL);
        if (shm_id == -1) {
            shm_id = shmget(key, size, 0666);
        }
        int* command = (int*) shmat(shm_id, nullptr, 0);
    #else
        int* command = (int*) calloc(length.NumberOfCommands + 3, sizeof(int));
    #endif // LINUX

    comparator(PtrToStr, length.NumberOfLines, fp, command);
    
    free(PtrToStr);
    
    return 0;
}

int comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* command) {
    char reg[4] = {};
    size_t NumbOfComs = 3;
    int dbg = 0;

    command[0] = signature;
    command[1] = version;
    
    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        int value = __INT_MAX__;
        char buff[20] = {};
        
        sscanf(PtrToStr[counter].ptrtostr, "%s%d", buff, &value);
        if (buff[0] == '\0') {
            sscanf(PtrToStr[++counter].ptrtostr, "%s%d", buff, &value);
        }
        //printf("%s\n", buff);
        #include "command_compare.h"
        else COMMAND_COMPARE("IN",   buff, STACK_IN)
        else COMMAND_COMPARE("POP",  buff, STACK_POP)
        else COMMAND_COMPARE("ADD",  buff, ADD)
        else COMMAND_COMPARE("SUB",  buff, SUB)
        else COMMAND_COMPARE("DIV",  buff, DIV)
        else COMMAND_COMPARE("SQRT", buff, SQRT)
        else COMMAND_COMPARE("SIN",  buff, SIN)
        else COMMAND_COMPARE("COS",  buff, COS)
        else COMMAND_COMPARE("OUT",  buff, OUT)
        else COMMAND_COMPARE("HLT",  buff, HLT)
        else {
            MyAssert(0 && "Syntax error!");
            Error = -1;
        }  
        if (command[counter - 1] == REG_ERR) {
            MyAssert(command[counter - 1] != REG_ERR && "You entered wrong register");
            Error = -1;
        }
    }
    command[2] = NumbOfComs - 3;

    fwrite(command, sizeof(int), NumbOfComs, PtrToCm);
    fclose(PtrToCm);
    
    FILE* fp = fopen("../cm.bin", "rb");
    int cm[NumbOfComs + 3];
    fread(cm, sizeof(int), NumbOfComs, fp);
    fclose(fp);
    for (size_t i = 0; i < NumbOfComs; i++) {
        printf("%d\n", cm[i]);
    }
    #if defined(LINUX)
        shmdt(command);
    #else
        free(command);
    #endif // LINUX

    return 0;
}

int disassembly(const char* DisAsmName, const char* CmName) {
    FILE* PtrToAsm = fopen(DisAsmName, "wb");
    FILE* PtrToCm  = fopen(CmName,     "rb");
    
    int tmp = __INT_MAX__;
    int cm[3] = {};

    fread(cm, sizeof(int), 3, PtrToCm);
    fwrite(cm, sizeof(int), 3, PtrToAsm);
    
    int* command = (int*) calloc(cm[2], sizeof(int));
    fread(command, sizeof(int), cm[2], PtrToCm);

    for (size_t counter = 0; counter < cm[2]; counter++) {
        switch (command[counter] & BITCOMM) {
        case (HLT & BITCOMM):
            fprintf(PtrToAsm, "HLT\n");
            break;
        case STACK_PUSH:
            fprintf(PtrToAsm, "PUSH ");
            tmp = command[counter + 1];
            counter++;
            fprintf(PtrToAsm, "%d\n", tmp);
            break;
        case STACK_IN:
            fprintf(PtrToAsm, "IN\n");
            break;
        case STACK_POP:
            fprintf(PtrToAsm, "POP\n");
            tmp = command[++counter];
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
            MyAssert(1 && "Wrong instruction");
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

int bitwise(char* BitComm, char* Identif) {
    for (size_t counter = 0; counter < 4; counter++) {
        *BitComm += 1 << counter;
        *Identif += 16 << counter;
    }
    return 0;
}

void my_assertion_failed(const char* condition, const char* file, const char* function, const int line) {
    printf(RED "Assertion failed on file %s on function %s:%d\n", file, function, line);
    printf("Wrong condition: %s\n", condition);
    printf(WHITE);
}