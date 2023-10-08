#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include "types.h"
                                                      

char   *InputBuffer(const char *FileName, Lengths *Length);
void InputPtrToBuffer(String *PtrToLine, Lengths *Length, char *Buffer);

#endif
