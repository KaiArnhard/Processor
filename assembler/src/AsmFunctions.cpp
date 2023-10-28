#include <cstdio>
#include "command.h"
#include "FileInput.h"
#include "AsmFunctions.h"

int RunAssembler(const char* PathToCm, const char* PathToAsm) {
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

    label_t label;
    label.inform = nullptr;
    label.inform = CtorLabel(&label);

    length.NumberOfCommands = Comparator(Ptr2Str, length.NumberOfLines, command, &label);

    for (size_t counter = 0; counter < length.NumberOfCommands; counter++) {
        int BitForm = command[counter] & BITCOMM;
        if (JUMP && BitForm == CMD_CALL && command[counter + 1] == DefaultLabelAdress) {
            length.NumberOfCommands = Comparator(Ptr2Str, length.NumberOfLines, command, &label);
            break;
        }   
    }
    InputCommsToFile(&label, command, PathToCm, length.NumberOfCommands);
    Destructor(&label, command, Ptr2Str);

    return 0;
}

int Comparator(String* PtrToStr, size_t NumbOfLines, int* command, label_t* label) {

    #define DEF_CMD(name, numb, arg, ...)                                                           \
        if (!strcmp(StrCommand, #name)) {                                                           \
            command[NumbOfComs] = CMD_##name;                                                       \
            NumbOfComs++;                                                                           \
            if(arg) {                                                                               \
                if(sscanf(PtrToStr[counter].str + strlen(#name), "%d", command + NumbOfComs)) {     \
                    command[NumbOfComs - 1] += immed;                                               \
                } else {                                                                            \
                    sscanf(PtrToStr[counter].str + strlen(#name), "%s", StrArgument);               \
                    CodingCommWithStrArg(StrArgument, label, command, NumbOfComs);                  \
                }                                                                                   \
                NumbOfComs++;                                                                       \
            }                                                                                       \
        } else if (strchr(StrCommand, ':')) {                                                       \
            label->counter = MakeLabel(label, StrCommand, NumbOfComs);                              \
        } else
        
    char StrCommand[StrCommandSize]  = {};
    char StrArgument[StrArgSize] = {};

    size_t NumbOfComs   = 0;
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
    PrintOfLabels(label);
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

void CodingCommWithStrArg(char *StrArgument, label_t* label, int* command, const int NumbOfComs) {
    char* left  = nullptr;
    char* right = nullptr;

    if (strchr(StrArgument, ':')) {
        command[NumbOfComs - 1] += immed;
        command[NumbOfComs] = LabelCheck(StrArgument, label);
    } else if((left = strchr(StrArgument, '[')) != nullptr && (right = strchr(StrArgument, ']')) != nullptr) {
        MakePtrToRAM(command, left, right, NumbOfComs);
    } else {
        MakeRegister(command, StrArgument, NumbOfComs);
    }
}

int MakePtrToRAM(int* command, char* left, char* right, const int NumbOfComs) {
    left++;
    
    size_t size = (size_t) (right - left);
    int tmp = 0;

    if (CheckOfDigit(left, size)) {
        command[NumbOfComs - 1] += RAMIdentif;
        command[NumbOfComs] = strtod(left, &right);
        return RAMIdentif;
    } else if ((tmp = MakeRegister(command, left, NumbOfComs)) == regis) {
        command[NumbOfComs - 1] += RAMIdentif;
        return RAMIdentif + immed;
    } else if (tmp == REG_ERR) {
        MyAssert(0 && "You entered wrong register", left);
        return REG_ERR;
    }
    
    return 0;
}

int CheckOfDigit(char* left, size_t size) {
    int tmp = 0;
    for (size_t counter = 0; counter < size; counter++) {
        tmp = isdigit(left[counter]);
        if (tmp == 0) {
            return tmp;
        }
    }
    return tmp;
}

int MakeRegister(int* command, const char* StrArgument, const int NumbOfComs) {
    if(StrArgument[0] != 'R' || StrArgument[2] != 'X' || (StrArgument[1] - 'A') > 3 || strlen(StrArgument) > 3) {
        command[NumbOfComs - 1]  = REG_ERR;
        return REG_ERR;
    } else {
        command[NumbOfComs - 1] += regis;
        command[NumbOfComs] = StrArgument[1] - 'A';
        return regis;
    }
}

int InputStrCommand(String* Ptr2Str, char* StrCommand, size_t counter) {
    sscanf(Ptr2Str[counter].str, "%s", StrCommand);
        if (StrCommand[0] == '\0') {
            sscanf(Ptr2Str[++counter].str, "%s", StrCommand);
        }
    return counter;
}

void InputCommsToFile(const label_t* label, const int* command, const char* PathToCm, const size_t NumbOfComs) {
    FILE* CommandFile = fopen(PathToCm, "wb");
    assert(CommandFile != nullptr);

    size_t tmp[3] = {signature, version, NumbOfComs};

    fwrite(tmp, sizeof(size_t), sizeof(tmp) / sizeof(size_t), CommandFile);
    fwrite(command, sizeof(int), NumbOfComs, CommandFile);
             
    fclose(CommandFile);
    PrintOfCommsFile(label, PathToCm, NumbOfComs);
}

void PrintOfCommsFile(const label_t* label, const char* PathToCm, const size_t NumbOfComs) {
    FILE* CommandFile = fopen(PathToCm, "rb");
    assert(CommandFile != nullptr);

    int command[NumbOfComs];
    size_t tmp[3] = {};
    
    fread(tmp, sizeof(size_t), sizeof(tmp) / sizeof(size_t), CommandFile);
    fread(command, sizeof(int), NumbOfComs, CommandFile);
    fclose(CommandFile);

    for (size_t counter = 0; counter < sizeof(tmp) / sizeof(size_t); counter++) {
        printf("%d\t", tmp[counter]);
    }
    printf("\n");
    for (size_t counter = 0; counter < NumbOfComs; counter++) {
        int BitForm = command[counter] & BITCOMM;
        if (BitForm == CMD_PUSH || BitForm == CMD_POP || JUMP || BitForm == CMD_CALL) {
            printf("%d  %d\n", command[counter], command[counter + 1]);
            counter++;
        }
        else {
            printf("%d\n", command[counter]);
        }
    }
    printf("\n");
    
}

void Destructor(label_t* label, int* command, String* PtrToStr) {
    #if defined(SHM)
        shmdt(command);
    #else
        free(command);
    #endif // SHM
    free(PtrToStr);
    free(label->inform);
}

int disassembly(const char* DisAsmName, const char* CmName) {
    FILE* PtrToAsm = fopen(DisAsmName, "w");
    assert(PtrToAsm != nullptr);
    FILE* PtrToCm  = fopen(CmName,     "rb");
    assert(PtrToCm != nullptr);
    int tmp = __INT_MAX__;
    size_t inf[3] = {};

    fread(inf, sizeof(size_t), 3, PtrToCm);

    for (size_t i = 0; i < 3; i++) {
        fprintf(PtrToAsm, "%d\n", inf[i]);
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
            fprintf(PtrToAsm, "POP ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
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
            fprintf(PtrToAsm, "JMP ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JA:
            fprintf(PtrToAsm, "JA ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JAE:
            fprintf(PtrToAsm, "JAE ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JB:
            fprintf(PtrToAsm, "JB ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JBE:
            fprintf(PtrToAsm, "JBE ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JE:
            fprintf(PtrToAsm, "JE ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_JNE:
            fprintf(PtrToAsm, "JNE ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;
        case CMD_CALL:
            fprintf(PtrToAsm, "CALL ");
            fprintf(PtrToAsm, "%d\n", command[++counter]);
            break;   
        case CMD_RET:
            fprintf(PtrToAsm, "RET\n");
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