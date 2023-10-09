#if !defined(COMMAND_H)
#define COMMAND_H

#include "types.h"

#if defined(LINUX)

    #include <sys/ipc.h>
    #include <sys/shm.h>
    const int proj_id = 1;

#endif // LINUX

#define RED    "\033[0;31m"
#define GREEN  "\e[0;32m"
#define YELLOW "\e[0;33m"
#define WHITE  "\e[0;37m"

#define MyAssert(condition)                                                                                      \
    ((bool) (condition) ? void (0) : my_assertion_failed(#condition, __FILE__, __PRETTY_FUNCTION__, __LINE__))   \

enum command_t {
    STACK_PUSH =  1,
    STACK_IN   =  2,
    STACK_POP  =  3,
    ADD        =  4,
    SUB        =  5,
    DIV        =  6,
    MUL        =  7,
    SQRT       =  8,
    SIN        =  9,
    COS        =  10,
    OUT        =  11,
    HLT        =  15 << 0 
};

enum REG_CODE {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3
};

static int Error = 0;
static const u_char REG_ERR = -1;
static const int signature = *((int*) "KLEO");
static const int version   = 2;

static const size_t immed = 1 << 4; //16
static const size_t regis = 1 << 5; //32

void my_assertion_failed(const char* condition, const char* file, const char* function, const int line);

int assembly(const char* PathToCm, const char* PathToAsm);
int disassembly(const char* DisAsmName, const char* CmName);
int comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* commands);
int bitwise(char* BitComm, char* Identif);

#endif // COMMAND_H