#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include "types.h"

#define COMMAND_COMPARE()

char   *InputBuffer(const char *FileName, Lengths *Length);
void InputPtrToBuffer(String *PtrToLine, Lengths *Length, char *Buffer);

void Output(String *PtrToLine, Lengths Length, char *FileName);
void OutputOrigin(char *Buffer, Lengths Length, char *FileName);

void comparator(String* PtrToStrs, size_t NumbOfLines, FILE* PtrToCm);

#endif
