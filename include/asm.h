#if !defined(ASM_H)
#define ASM_H

#include "types.h"

static const u_char REG_ERR = -1;

struct label_t {
    char point[100];
    int addres;
}; //TODO sourse address

static const size_t ResizeLabelConst    =  2;
static const size_t MaxLabelSize        =  20;
static const size_t DefaultLabelSize    =  10;
static const int DefaultLabelAdress     = -1;

int RunAssembler(const char* PathToCm, const char* PathToAsm);
int disassembly(const char* DisAsmName, const char* CmName);
int Comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* commands, label_t* label);

void InputAsmToFile(const label_t* label, const int* command, FILE* CommandFile, const size_t NumbOfComs);
void DbgPrintOfAsmedFile(const label_t* label, const int* command, FILE* CommandFile, const size_t NumbOfComs);

void Destructor(label_t* label, int* command, String* PtrToStr);

label_t* CtorLabel(label_t* label);
label_t* ResizeLabel(label_t* label);
int LabelCheck(char* buffer, label_t* label, size_t LabelCounter);

#endif // ASM_H
