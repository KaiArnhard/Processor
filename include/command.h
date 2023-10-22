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

enum POP_PUSH_CODES {
    BITCOMM = (15 << 0) + (1 << 4),
    IDENTIF = 7  << 5
};

static int Error = 0;

static const size_t signature = *((int*) "KLEO");
static const size_t version   = 3;

static const size_t immed = 1 << 5; //32
static const size_t regis = 1 << 6; //64

#if defined(SHM)
    int* ShmCtor(char *PathToCm, Lengths* length);
#endif // SHM


void MyAssertionFailed(const char* condition, const char* command, const char* file, const char* function, const int line);

#endif // COMMAND_H