#if !defined(ASM_FUNCTIONS_H)
#define ASM_FUNCTIONS_H

#include "types.h"
#include "labels.h"

#define JUMP                                                                                \
    BitForm == CMD_JMP || BitForm == CMD_JA || BitForm == CMD_JAE ||                        \
    BitForm == CMD_JB || BitForm == CMD_JBE || BitForm == CMD_JE || BitForm == CMD_JNE

static const u_char REG_ERR = -1;

static const size_t StrCommandSize    = 100;
static const size_t StrArgSize        = 100;
static const size_t ResizeLabelConst  =  2;
static const size_t MaxLabelSize      =  80;
static const size_t DefaultLabelSize  =  10;
static const int DefaultLabelAdress   = -1;

int RunAssembler(const char* PathToCm, const char* PathToAsm);
int Comparator(String* PtrToStr, size_t NumbOfLines, int* commands, label_t* label);

int InputStrCommand(String* Ptr2Str, char* StrCommand, size_t counter);

void CodingCommWithStrArg(char *StrArgument, label_t* label, int* command, const int NumbOfComs);
int MakePtrToRAM(int* command, char* left, char* right, const int NumbOfComs);
int MakeRegister(int* command, const char* StrArgument, const int NumbOfComs);

void InputCommsToFile(const label_t* label, const int* command, const char* PathToCm, const size_t NumbOfComs);
void PrintOfCommsFile(const label_t* label, const char* PathToCm, const size_t NumbOfComs);

void Destructor(label_t* label, int* command, String* PtrToStr);

int disassembly(const char* DisAsmName, const char* CmName);

int CheckOfDigit(char* left, size_t size);

#endif // ASM_FUNCTIONS_H
