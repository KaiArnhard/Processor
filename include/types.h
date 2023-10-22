#ifndef TYPES_H
#define TYPES_H

#include <ctype.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

struct String
{
    char *str        = 0;
    int  LengthOfStr = 0;
};
struct Lengths
{
    size_t NumberOfLines    = 0;
    size_t LengthOfBuffer   = 0;
    size_t NumberOfCommands = 0;
};

enum Solutions
{
    MORE = 1,
    LESS = -1,
    EQUAL = 0,
};

#endif
