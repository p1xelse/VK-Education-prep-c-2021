#include <stdlib.h>
#include <stdio.h>
#include "eml_parc.h"


int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];
    
    FILE *f = fopen(path_to_eml, "r");
    file_eml_parse(f);
    fclose(f);

    return 0;
}
