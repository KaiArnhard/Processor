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

int assembly(const char* fname1, const char* fname2) {
    FILE* fp = fopen(fname1, "w");
    assert(fp != nullptr);
    String* PtrToStr = nullptr;
    Lengths length = {};

    char* buffer = InputBuffer(fname2, &length);
    
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
        int* command = (int*) calloc(length.NumberOfCommands, sizeof(int));
    #endif // LINUX

    comparator(PtrToStr, length.NumberOfLines, fp, command);
    
    free(PtrToStr);
    //fclose(fp);
    
    return 0;
}

void comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* command) {
    char buff[20] = {};
    char reg[4] = {};
    size_t NumbOfComs = 3;
    int dbg = 0;

    for (size_t counter = 0; counter < sizeof(int); counter++) {
        ((char*)command)[counter] = *((char*) &signature + counter);
    }
    command[1] = version;

    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        int value = __INT_MAX__;
        sscanf(PtrToStr[counter].ptrtostr, "%s%d", buff, &value);

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
            printf(RED);
            printf("Syntax Error\n");
            printf("Entered command is %s\n", buff);
            #if defined(LINUX)
                shmdt(command);
            #endif // LINUX
            abort();
        } 

        if (command[NumbOfComs - 1] == REG_ERR) {
            printf(RED);
            printf("You entered wrong register, it is %s\n", reg);
            #if defined(LINUX)
                shmdt(command);
            #endif // LINUX
            abort();
        }
    }
    command[2] = NumbOfComs - 3;

    fwrite(command, sizeof(int), NumbOfComs, PtrToCm);
    fclose(PtrToCm);
    
    FILE* fp = fopen(PathToCm1, "r");
    int cm[NumbOfComs];
    fread(cm, sizeof(int), NumbOfComs, fp);
    fclose(fp);
    for (size_t i = 0; i < NumbOfComs; i++) {
        printf("%d\t", cm[i]);
    }
    

    #if defined(LINUX)
        shmdt(command);
    #else
        free(command);
    #endif // LINUX
}

int disassembly(const char* DisAsmName, const char* CmName) {
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