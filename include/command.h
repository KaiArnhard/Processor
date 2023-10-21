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


#define JUMP                                                                                \
    BitForm == CMD_JMP || BitForm == CMD_JA || BitForm == CMD_JAE ||                        \
    BitForm == CMD_JB || BitForm == CMD_JBE || BitForm == CMD_JE || BitForm == CMD_JNE

enum POP_PUSH_CODES {
    BITCOMM = (15 << 0) + (1 << 4),
    IDENTIF = 7  << 5
};

struct label_t {
    char point[100] = { };
    int addres = -1;
};

static const size_t resize_label_const    =  2;
static const size_t max_label_size        =  20;
static const size_t default_label_size    =  10;
static const int default_label_adress     = -1;
static const size_t default_commands_size =  3;

static int Error = 0;
static const u_char REG_ERR = -1;

static const int signature = *((int*) "KLEO");
static const int version   = 3;

static const size_t immed = 1 << 5; //32
static const size_t regis = 1 << 6; //64

void my_assertion_failed(const char* condition, const char* command, const char* file, const char* function, const int line);

int assembly(const char* PathToCm, const char* PathToAsm);
int disassembly(const char* DisAsmName, const char* CmName);
int comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* commands, label_t* label);

void PrintOfAsm(const label_t* label, const int* command, FILE* CommandFile, const size_t NumbOfComs);
void Destructor(label_t* label, int* command, String* PtrToStr);

label_t* CtorLabel(label_t* label);
label_t* ResizeLabel(label_t* label);
int label_check(char* buffer, label_t* label, size_t label_counter);

#endif // COMMAND_H