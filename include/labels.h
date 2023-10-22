#if !defined(LABELS_H)
#define LABELS_H

#include "types.h"

struct label_t {
    char point[100];
    int  DestAddres;
    int  SourceAddres[10];
    size_t LabelSize = 0;
}; //TODO sourse address

label_t* CtorLabel(label_t* label);
label_t* ResizeLabel(label_t* label);

int LabelCheck(char* NameOfLabel, label_t* label, size_t LabelCounter);
int MakeLabel(label_t* label, char* NameOfLabel, size_t LabelCounter, size_t NumbOfComs);

#endif // LABELS_H
