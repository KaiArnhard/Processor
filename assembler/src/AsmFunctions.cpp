#include <cstdio>
#include "command.h"
#include "FileInput.h"
#include "asm.h"

int RunAssembler(const char* PathToCm, const char* PathToAsm) {
    FILE* CommandFile = fopen(PathToCm, "wb");
    assert(CommandFile != nullptr);

    String* Ptr2Str = nullptr;
    Lengths length = {};

    char* buffer = ReadFileToBuffer(PathToAsm, &length);
    
    Ptr2Str = (String*) calloc(length.NumberOfLines, sizeof(String));
    
    InputPtrToBuffer(Ptr2Str, &length, buffer);
    
    #if defined(SHM)
        int *command = ShmCtor(PathToCm, &length);
    #else
        int* command = (int*) calloc(length.NumberOfCommands, sizeof(int));
    #endif // SHM

    label_t* label = nullptr;
    label = CtorLabel(label);

    length.NumberOfCommands = Comparator(Ptr2Str, length.NumberOfLines, CommandFile, command, label);

    for (size_t counter = 0; counter < length.NumberOfCommands; counter++) {
        int BitForm = command[counter] & BITCOMM;
        if (JUMP && command[counter + 1] == DefaultLabelAdress) {
            length.NumberOfCommands = Comparator(Ptr2Str, length.NumberOfLines, CommandFile, command, label);
            break;
        }   
    }
    InputCommsToFile(label, command, CommandFile, length.NumberOfCommands);
    Destructor(label, command, Ptr2Str);

    return 0;
}

int Comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* command, label_t* label) {

    #define DEF_CMD(name, numb, arg, ...)                                                           \
        if (!strcmp(StrCommand, #name)) {                                                           \
            command[NumbOfComs] = CMD_##name;                                                       \
            NumbOfComs++;                                                                           \
            if(arg) {                                                                               \
                if(sscanf(PtrToStr[counter].str + strlen(#name), "%d", command + NumbOfComs)) {     \
                    command[NumbOfComs - 1] += immed;                                               \
                } else {                                                                            \
                    sscanf(PtrToStr[counter].str + strlen(#name), "%s", StrArgument);               \
                    CodingCommWithStrArg(StrArgument, label, command, NumbOfComs, LabelCounter);    \
                }                                                                                   \
                NumbOfComs++;                                                                       \
            }                                                                                       \
        } else if (strchr(StrCommand, ':')) {                                                       \
            LabelCounter = MakeLabel(label, StrCommand, LabelCounter, NumbOfComs);                  \
        } else
        
    char StrCommand[StrCommandSize]  = {};
    char StrArgument[StrArgSize] = {};

    size_t NumbOfComs   = 0;
    size_t LabelCounter = 0;
    int verify          = 0;                                             
    
    for (size_t counter = 0; counter < NumbOfLines; counter++) {

        counter = InputStrCommand(PtrToStr, StrCommand, counter);
        
        #include "dsl.h"
        /*else*/ {
            MyAssert(command[counter] == __INT_MAX__ && "Syntax error!", StrArgument);
            Error = -1;
        } if (command[counter - 1] == REG_ERR) {
            MyAssert(command[counter - 1] != REG_ERR && "You entered wrong register!", StrCommand);
            Error = -1;
        }   
    }
    return NumbOfComs;
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

void CodingCommWithStrArg(char *StrArgument, label_t* label, int* command, const int NumbOfComs, const int LabelCounter) {
    if (strchr(StrArgument, ':')) {
        command[NumbOfComs - 1] += immed;
        command[NumbOfComs] = LabelCheck(StrArgument, label, LabelCounter);
    } else if(StrArgument[0] != 'R' || StrArgument[2] != 'X' || (StrArgument[1] - 'A') > 3 || strlen(StrArgument) > 3) {
        command[NumbOfComs - 1]  = REG_ERR;
    } else {
        command[NumbOfComs - 1] += regis;
        command[NumbOfComs] = StrArgument[1] - 'A';
    }
}

int InputStrCommand(String* Ptr2Str, char* StrCommand, size_t counter) {
    sscanf(Ptr2Str[counter].str, "%s", StrCommand);
        if (StrCommand[0] == '\0') {
            sscanf(Ptr2Str[++counter].str, "%s", StrCommand);
        }
    return counter;
}

void InputCommsToFile(const label_t* label, const int* command, FILE* CommandFile, const size_t NumbOfComs) {
    size_t tmp[3] = {signature, version, NumbOfComs};

    fwrite(tmp, sizeof(size_t), sizeof(tmp) / sizeof(size_t), CommandFile);
    fwrite(command, sizeof(int), NumbOfComs, CommandFile);

    PrintOfCommsFile(label, CommandFile, NumbOfComs);             
}

void PrintOfCommsFile(const label_t* label, FILE* CommandFile, const size_t NumbOfComs) {
    fclose(CommandFile);
    FILE* fp1 = fopen("../cm.bin", "rb");
    int command[NumbOfComs];
    size_t tmp[3] = {};
    
    fread(tmp, sizeof(size_t), sizeof(tmp) / sizeof(size_t), fp1);
    fread(command, sizeof(int), NumbOfComs, fp1);
    fclose(fp1);

    for (size_t counter = 0; counter < sizeof(tmp) / sizeof(size_t); counter++) {
        printf("%d\t", tmp[counter]);
    }
    printf("\n");
    for (size_t counter = 0; counter < NumbOfComs; counter++) {
        int BitForm = command[counter] & BITCOMM;
        if (BitForm == CMD_PUSH || BitForm == CMD_POP || JUMP) {
            printf("%d  %d\n", command[counter], command[counter + 1]);
            counter++;
        }
        else {
            printf("%d\n", command[counter]);
        }
    }
    printf("\n");
    for (size_t counter = 0; counter < label->LabelSize; counter++) {
        printf("%d  %s  %d\n", counter, label[counter].point, label[counter].DestAddres);
    }
}

int LabelCheck(char* NameOfLabel, label_t* label, size_t LabelCounter) {
    for (size_t counter = 0; counter <= LabelCounter; counter++) {
        if (!strcmp(NameOfLabel, label[counter].point)) {
            return label[counter].DestAddres;
        }
    }
    return DefaultLabelAdress;
}

int MakeLabel(label_t* label, char* NameOfLabel, size_t LabelCounter, size_t NumbOfComs) {
    if (LabelCounter >= label->LabelSize) {
            label = ResizeLabel(label);
    } else if (LabelCounter >= MaxLabelSize - 1) {
        printf(RED "Labels are full\n");
    } else {
        strcpy(label[LabelCounter].point, NameOfLabel);
        label[LabelCounter].DestAddres = NumbOfComs;
        LabelCounter++;
    }
    return LabelCounter;
}

void Destructor(label_t* label, int* command, String* PtrToStr) {
    #if defined(SHM)
        shmdt(command);
    #else
        free(command);
    #endif // SHM
    free(PtrToStr);
    free(label);
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