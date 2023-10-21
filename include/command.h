#if !defined(COMMAND_H)
#define COMMAND_H

#if defined(LINUX)

    #include <sys/ipc.h>
    #include <sys/shm.h>
    const int ProjId = 1;

#endif // LINUX

#include "types.h"

#define RED    "\033[0;31m"
#define GREEN  "\e[0;32m"
#define YELLOW "\e[0;33m"
#define WHITE  "\e[0;37m"
#define BLUE   "\e[0;34m"

#define MyAssert(condition, command)                                                                                       \
    ((bool) (condition) ? void (0) : MyAssertionFailed(#condition, command, __FILE__, __PRETTY_FUNCTION__, __LINE__))    \

#define DEF_CMD(name, numb, ...)    \
    CMD_##name = numb,

enum command_t {
    #include "dsl.h"
};
#undef DEF_CMD


#define JUMP                                                                                \
    BitForm == CMD_JMP || BitForm == CMD_JA || BitForm == CMD_JAE ||                        \
    BitForm == CMD_JB || BitForm == CMD_JBE || BitForm == CMD_JE || BitForm == CMD_JNE

enum POP_PUSH_CODES {
    BITCOMM = (15 << 0) + (1 << 4),
    IDENTIF = 7  << 5
};

struct label_t {
    char point[100];
    int addres;
};

static const size_t ResizeLabelConst    =  2;
static const size_t MaxLabelSize        =  20;
static const size_t DefaultLabelSize    =  10;
static const int DefaultLabelAdress     = -1;

static int Error = 0;
static const u_char REG_ERR = -1;

static const int signature = *((int*) "KLEO");
static const int version   = 3;

static const size_t immed = 1 << 5; //32
static const size_t regis = 1 << 6; //64

#if defined(SHM)
    int* ShmCtor(char *PathToCm, Lengths* length);
#endif // SHM


void MyAssertionFailed(const char* condition, const char* command, const char* file, const char* function, const int line);

int RunAssembler(const char* PathToCm, const char* PathToAsm);
int disassembly(const char* DisAsmName, const char* CmName);
int Comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* commands, label_t* label);

void InputAsmToFile(const label_t* label, const int* command, FILE* CommandFile, const size_t NumbOfComs);
void Destructor(label_t* label, int* command, String* PtrToStr);

label_t* CtorLabel(label_t* label);
label_t* ResizeLabel(label_t* label);
int LabelCheck(char* buffer, label_t* label, size_t LabelCounter);

#endif // COMMAND_H