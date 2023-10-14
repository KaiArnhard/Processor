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
#define BLUE   "\e[0;34m"

#define MyAssert(condition, command)                                                                                       \
    ((bool) (condition) ? void (0) : my_assertion_failed(#condition, command, __FILE__, __PRETTY_FUNCTION__, __LINE__))    \

#define DEF_CMD(name, numb, ...)    \
    CMD_##name = numb,

enum command_t {
    #include "dsl.h"
};
#undef DEF_CMD

enum POP_PUSH_CODES {
    BITCOMM = 15 << 0,
    IDENTIF = 15 << 4
};

static int Error = 0;
static const u_char REG_ERR = -1;
static const int signature = *((int*) "KLEO");
static const int version   = 2;

static const size_t immed = 1 << 4; //16
static const size_t regis = 1 << 5; //32

void my_assertion_failed(const char* condition, const char* command, const char* file, const char* function, const int line);

int assembly(const char* PathToCm, const char* PathToAsm);
int disassembly(const char* DisAsmName, const char* CmName);
int comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* commands);

#endif // COMMAND_H