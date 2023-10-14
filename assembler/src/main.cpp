#include "command.h"
#include "FileInput.h"
#include <cstring>

int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf(RED "You don't entered names of files\n");
    } else if (argc == 2) {
        printf(RED "You entered name of file %s\n", argv[1]);
    } else if (argc > 4){
        printf(RED "You entered too many names of files\n");
    }
    
    assembly(argv[2], argv[1]);
    return Error;
}