#include <cstdio>
#include "command.h"
#include "FileInput.h"
#include <cstring>

int assembly(const char* PathToCm, const char* PathToAsm) {
    FILE* CommandFile = fopen(PathToCm, "wb");
    assert(CommandFile != nullptr);

    String* PtrToStr = nullptr;
    Lengths length = {};

    char* buffer = InputBuffer(PathToAsm, &length);
    
    PtrToStr = (String*) calloc(length.NumberOfLines, sizeof(String));
    
    InputPtrToBuffer(PtrToStr, &length, buffer);
    
    #if defined(SHM)
        key_t key = ftok(PathToCm, proj_id);
        size_t size = (4 + length.NumberOfCommands) * sizeof(int);
        int shm_id = shmget(key, size, 0666 | IPC_CREAT |IPC_EXCL);
        if (shm_id == -1) {
            shm_id = shmget(key, size, 0666);
        }
        int* command = (int*) shmat(shm_id, nullptr, 0);
    #else
        int* command = (int*) calloc(length.NumberOfCommands + 3, sizeof(int));
    #endif // SHM

    label_t* label;
    label = CtorLabel(label);

    comparator(PtrToStr, length.NumberOfLines, CommandFile, command, label);

    size_t NumbOfComs = command[2] + default_commands_size;

    for (size_t counter = 0; counter < NumbOfComs; counter++) {
        int BitForm = command[counter] & BITCOMM;
        if (JUMP && command[counter + 1] == -1) {
            comparator(PtrToStr, length.NumberOfLines, CommandFile, command, label);
            break;
        }   
    }
    PrintOfAsm(label, command, CommandFile, NumbOfComs);
    Destructor(label, command, PtrToStr);

    return 0;
}

int comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* command, label_t* label) {

    #define DEF_CMD(name, numb, arg, ...)                                                                                       \
        if (!strcmp(buffer, #name)) {                                                                                           \
            command[NumbOfComs] = CMD_##name;                                                                                   \
            NumbOfComs++;                                                                                                       \
            if(arg) {                                                                                                           \
                if(sscanf(PtrToStr[counter].ptrtostr + strlen(#name), "%d", command + NumbOfComs))  {                           \
                    command[NumbOfComs - 1] += immed;                                                                           \
                } else if ((verify = sscanf(PtrToStr[counter].ptrtostr + strlen(#name), "%s", buff)) != 0 && verify != EOF) {   \
                    if (strchr(buff, ':')) {                                                                                    \
                        command[NumbOfComs - 1] += immed;                                                                       \
                        command[NumbOfComs] = label_check(buff, label, label_counter);                                          \
                    } else if(buff[0] != 'R' || buff[2] != 'X' || (buff[1] - 'A') > 3 || strlen(buff) > 3) {                    \
                        command[NumbOfComs - 1] = REG_ERR;                                                                      \
                    } else {                                                                                                    \
                    command[NumbOfComs - 1] += regis;                                                                           \
                    command[NumbOfComs] = buff[1] - 'A';                                                                        \
                }                                                                                                               \
            }                                                                                                                   \
                NumbOfComs++;                                                                                                   \
            }                                                                                                                   \
        } else if (strchr(buffer, ':')) {                                                                                       \
            strcpy(label[label_counter].point, buffer);                                                                         \
            label[label_counter].addres = NumbOfComs - default_commands_size;                                                   \
            label_counter++;                                                                                                    \
        } else
        
    char buff[100]   = {};

    size_t NumbOfComs = default_commands_size;
    size_t label_counter = 0;
    int verify = 0;

    command[0] = signature;
    command[1] = version;                                             
    
    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        char buffer[100] = {};
        sscanf(PtrToStr[counter].ptrtostr, "%s", buffer);
        
        if (buffer[0] == '\0') {
            sscanf(PtrToStr[++counter].ptrtostr, "%s", buffer);
        }
        if (label_counter == default_label_size - 1) {
            label = ResizeLabel(label);
        } else if (label_counter == max_label_size - 1) {
            printf(RED "Labels are full\n");
        }
        
        #include "dsl.h"
        /*else*/ {
            MyAssert(command[counter] == __INT_MAX__ && "Syntax error!", buffer);
            Error = -1;
        } if (command[counter - 1] == REG_ERR) {
            MyAssert(command[counter - 1] != REG_ERR && "You entered wrong register!", buff);
            Error = -1;
        }   
    }
    command[2] = NumbOfComs - default_commands_size;

    return 0;
}

void Destructor(label_t* label, int* command, String* PtrToStr) {
    free(PtrToStr);
    
    #if defined(SHM)
        shmdt(command);
    #else
        free(command);
    #endif // SHM
    free(label);
}

void PrintOfAsm(const label_t* label, const int* command, FILE* CommandFile, const size_t NumbOfComs) {
    fwrite(command, sizeof(int), NumbOfComs, CommandFile);
    fclose(CommandFile);
    
    FILE* fp1 = fopen("../cm.bin", "rb");
    int FCommand[NumbOfComs];
    fread(FCommand, sizeof(int), NumbOfComs, fp1);
    fclose(fp1);

    size_t counter = 0;
    for (; counter < default_commands_size; counter++) {
        printf("%d\t", FCommand[counter]);
    }
    printf("\n");
    for (; counter < NumbOfComs; counter++) {
        int BitForm = FCommand[counter] & BITCOMM;
        if (BitForm == CMD_PUSH || BitForm == CMD_POP || JUMP) {
            printf("%d  %d\n", FCommand[counter], FCommand[counter + 1]);
            counter++;
        }
        else {
            printf("%d\n", FCommand[counter]);
        }
    }
    for (size_t i = 0; i < 10; i++) {
        printf("%d\t%s\n", label[i].addres, label[i].point);
    }
}

label_t* CtorLabel(label_t* label) {
    label = (label_t*) calloc(default_label_size, sizeof(label_t));
    for (size_t counter = 0; counter < default_label_size; counter++) {
        label[counter].addres = default_label_adress;
    }
    return label;
}

label_t* ResizeLabel(label_t* label) {
    label = (label_t*) realloc(label, sizeof(label_t) * resize_label_const * default_label_size);
    return label;
}

int label_check(char* buffer, label_t* label, size_t label_counter) {
    for (size_t counter = 0; counter <= label_counter; counter++) {
        if (!strcmp(buffer, label[counter].point)) {
            return label[counter].addres;
        }
    }
    return -1;
}

/*int disassembly(const char* DisAsmName, const char* CmName) {
    FILE* PtrToAsm = fopen(DisAsmName, "w");
    FILE* PtrToCm  = fopen(CmName,     "r");
    
    int tmp = __INT_MAX__;
    int cm[3] = {};

    fread(cm, sizeof(int), 3, PtrToCm);
    fwrite(cm, sizeof(int), 3, PtrToAsm);

    for (size_t i = 0; i < 3; i++) {
        printf("%d\n", cm[i]);
    }
    
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
}*/

void my_assertion_failed(const char* condition, const char* command, const char* file, const char* function, const int line) {
    printf(RED "Assertion failed on file %s on function %s:%d\n", file, function, line);
    printf("Wrong condition: %s\n", condition);
    printf("Intered %s\n", command);
    printf(WHITE);
}