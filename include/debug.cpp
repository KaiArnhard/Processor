# 0 "command.h"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "command.h"
# 25 "command.h"
enum command_t {
# 1 "dsl.h" 1
CMD_HLT = -1,
CMD_PUSH = 0,
CMD_POP = 1,
CMD_IN = 2,
CMD_ADD = 3,
CMD_SUB = 4,
CMD_MUL = 5,
CMD_DIV = 6,
CMD_SQRT = 7,
CMD_SIN = 8,
CMD_COS = 9,
CMD_OUT = 10,
# 27 "command.h" 2
};


enum POP_PUSH_CODES {
    BITCOMM = 15 << 0,
    IDENTIF = 15 << 4
};

static int Error = 0;
static const u_char REG_ERR = -1;
static const int signature = *((int*) "KLEO");
static const int version = 2;

static const size_t immed = 1 << 4;
static const size_t regis = 1 << 5;

void my_assertion_failed(const char* condition, const int command, const char* file, const char* function, const int line);

int assembly(const char* PathToCm, const char* PathToAsm);
int disassembly(const char* DisAsmName, const char* CmName);
int comparator(String* PtrToStr, size_t NumbOfLines, FILE* PtrToCm, int* commands);
