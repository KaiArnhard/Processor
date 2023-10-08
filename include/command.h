#if !defined(COMMAND_H)
#define COMMAND_H

#include "types.h"

#if defined(LINUX)

    #include <sys/ipc.h>
    #include <sys/shm.h>
    const int proj_id = 1;

#endif // LINUX

enum command_t {
    STACK_PUSH =  0,
    STACK_IN   =  1,
    STACK_POP  =  2,
    ADD        =  3,
    SUB        =  4,
    DIV        =  5,
    MUL        =  6,
    SQRT       =  7,
    SIN        =  8,
    COS        =  9,
    OUT        =  10,
    HLT        =  15 << 0 
};

enum REG_CODE {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3
};

static const u_char REG_ERR = -1;
static const int signature = *((int*) "KLEO");
static const int version   = 2;

static const size_t immed = 1 << 4; //16
static const size_t regis = 1 << 5; //32

static const char* PathToCm = "home/kali/Processor/cm.bin";
static const char* PathToCm1 = "/home/kali/Processor/cm.bin";

static const char* RED    = "\033[0;31m";
static const char* GREEN  = "\e[0;32m";
static const char* YELLOW = "\e[0;33m";
static const char* WHITE  = "\e[0;37m";

int assembly(const char* fname1, const char* fname2);
int disassembly(const char* DisAsmName, const char* CmName);
void comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* commands);

#endif // COMMAND_H