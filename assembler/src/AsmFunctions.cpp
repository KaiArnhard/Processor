#include <cstdio>
#include "command.h"
#include "FileInput.h"
#include <cstring>

int RunAssembler(const char* PathToCm, const char* PathToAsm) {
    FILE* CommandFile = fopen(PathToCm, "wb");
    assert(CommandFile != nullptr);

    String* PtrToStr = nullptr;
    Lengths length = {};

    char* buffer = ReadFileToBuffer(PathToAsm, &length);
    
    PtrToStr = (String*) calloc(length.NumberOfLines, sizeof(String));
    
    InputPtrToBuffer(PtrToStr, &length, buffer);
    
    #if defined(SHM)
        int *command = ShmCtor(PathToCm, &length);
    #else
        int* command = (int*) calloc(length.NumberOfCommands + 3, sizeof(int));
    #endif // SHM

    label_t* label = nullptr;
    label = CtorLabel(label);
    assert(label != nullptr && "Pointer to label is null");

    Comparator(PtrToStr, length.NumberOfLines, CommandFile, command, label);

    size_t NumbOfComs = command[2] + DefaultCommandsSize;

    for (size_t counter = 0; counter < NumbOfComs; counter++) {
        int BitForm = command[counter] & BITCOMM;
        if (JUMP && command[counter + 1] == -1) {
            Comparator(PtrToStr, length.NumberOfLines, CommandFile, command, label);
            break;
        }   
    }
    PrintOfAsm(label, command, CommandFile, NumbOfComs);
    Destructor(label, command, PtrToStr);

    return 0;
}

int Comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* command, label_t* label) {

    #define DEF_CMD(name, numb, arg, ...)                                                                                                   \
        if (!strcmp(StrCommand, #name)) {                                                                                                   \
            command[NumbOfComs] = CMD_##name;                                                                                               \
            NumbOfComs++;                                                                                                                   \
            if(arg) {                                                                                                                       \
                if(sscanf(PtrToStr[counter].ptrtostr + strlen(#name), "%d", command + NumbOfComs))  {                                       \
                    command[NumbOfComs - 1] += immed;                                                                                       \
                } else if ((verify = sscanf(PtrToStr[counter].ptrtostr + strlen(#name), "%s", StrArgument)) != 0 && verify != EOF) {        \
                    if (strchr(StrArgument, ':')) {                                                                                         \
                        command[NumbOfComs - 1] += immed;                                                                                   \
                        command[NumbOfComs] = LabelCheck(StrArgument, label, LabelCounter);                                                 \
                    } else if(StrArgument[0] != 'R' || StrArgument[2] != 'X' || (StrArgument[1] - 'A') > 3 || strlen(StrArgument) > 3) {    \
                        command[NumbOfComs - 1] = REG_ERR;                                                                                  \
                    } else {                                                                                                                \
                        command[NumbOfComs - 1] += regis;                                                                                   \
                        command[NumbOfComs] = StrArgument[1] - 'A';                                                                         \
                }                                                                                                                           \
            }                                                                                                                               \
                NumbOfComs++;                                                                                                               \
            }                                                                                                                               \
        } else if (strchr(StrCommand, ':')) {                                                                                               \
            strcpy(label[LabelCounter].point, StrCommand);                                                                                  \
            label[LabelCounter].addres = NumbOfComs - DefaultCommandsSize;                                                                  \
            LabelCounter++;                                                                                                                 \
        } else
        
    char StrArgument[100]   = {};

    size_t NumbOfComs = DefaultCommandsSize;
    size_t LabelCounter = 0;
    int verify = 0;

    command[0] = signature;
    command[1] = version;                                             
    
    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        char StrCommand[100] = {};
        sscanf(PtrToStr[counter].ptrtostr, "%s", StrCommand);
        
        if (StrCommand[0] == '\0') {
            sscanf(PtrToStr[++counter].ptrtostr, "%s", StrCommand);
        }
        if (LabelCounter == DefaultLabelSize - 1) {
            label = ResizeLabel(label);
        } else if (LabelCounter == MaxLabelSize - 1) {
            printf(RED "Labels are full\n");
        }
        
        #include "dsl.h"
        /*else*/ {
            MyAssert(command[counter] == __INT_MAX__ && "Syntax error!", StrArgument);
            Error = -1;
        } if (command[counter - 1] == REG_ERR) {
            MyAssert(command[counter - 1] != REG_ERR && "You entered wrong register!", StrCommand);
            Error = -1;
        }   
    }
    command[2] = NumbOfComs - DefaultCommandsSize;

    return 0;
}

#if defined(SHM)
    int* ShmCtor(char *PathToCm, Lengths* length) {
        key_t key = ftok(PathToCm, ProjId);
        size_t size = (4 + length->NumberOfCommands) * sizeof(int);
        int ShmId = shmget(key, size, 0666 | IPC_CREAT |IPC_EXCL);
        if (ShmId == -1) {
            ShmId = shmget(key, size, 0666);
        }
        return (int*) shmat(ShmId, nullptr, 0);
    }
#endif // SHM

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
    for (; counter < DefaultCommandsSize; counter++) {
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
    label = (label_t*) calloc(DefaultLabelSize, sizeof(label_t));
    for (size_t counter = 0; counter < DefaultLabelSize; counter++) {
        label[counter].addres = DefaultLabelAdress;
    }
    return label;
}

label_t* ResizeLabel(label_t* label) {
    label = (label_t*) realloc(label, sizeof(label_t) * ResizeLabelConst * DefaultLabelSize);
    for (size_t counter = DefaultLabelSize; counter < ResizeLabelConst * DefaultLabelSize; counter++) {
        label[counter].addres = -1;
    }
    
    return label;
}

int LabelCheck(char* buffer, label_t* label, size_t LabelCounter) {
    for (size_t counter = 0; counter <= LabelCounter; counter++) {
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

void MyAssertionFailed(const char* condition, const char* command, const char* file, const char* function, const int line) {
    printf(RED "Assertion failed on file %s on function %s:%d\n", file, function, line);
    printf("Wrong condition: %s\n", condition);
    printf("Intered %s\n", command);
    printf(WHITE);
}