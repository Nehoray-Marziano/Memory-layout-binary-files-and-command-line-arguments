#include <ctype.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
void PrintHex(unsigned char *buffer, int length){
    int i;
    for(i=0;i<length;i++)
        printf("%x ",buffer[i]);
}
void hexaPrint(char *link){
    int             size = 0;  /* number of characters seen */
    FILE           *in_file;    /* input file */

    /* character or EOF flag from input */
    int             ch;

    in_file = fopen(link, "rb");
    if (in_file == NULL) {
        printf("Cannot open %s\n", link);
        exit(8);
    }

    while (1) {
        ch = fgetc(in_file);
        if (ch == EOF)
            break;
        ++size;
    }
    in_file=fopen(link,"rb");
    unsigned char *buffer=malloc(size*sizeof(char));
    fread(buffer,1,size,in_file);
    PrintHex(buffer,size);
    free(buffer);

}

int main(int argc, char **argv) {
hexaPrint(argv[1]);
}