#ifndef STRING_H
#define STRING_H

#include <ctype.h>
#include <cstdlib>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

struct String
{
    char *ptrtostr   = 0;
    int  lengthofstr = 0;
};
struct Lengths
{
    int NumberOfLines   = 0;
    int LengthOfBuffer  = 0;
};

enum Solutions
{
    MORE = 1,
    LESS = -1,
    EQUAL = 0,
};

size_t Mystrlen(const char *str);
int isletter(const unsigned char *str);

#endif
