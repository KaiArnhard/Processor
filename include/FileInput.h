#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include "types.h"

static const size_t SizeOf1Command = 4;
         
char   *ReadFileToBuffer(const char *FileName, Lengths *Length);
void InputPtrToBuffer(String *PtrToLine, Lengths *Length, char *Buffer);

#endif
