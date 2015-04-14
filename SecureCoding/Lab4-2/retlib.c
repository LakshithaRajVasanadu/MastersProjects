#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int bof(FILE *badfile)
{
    char buffer[48];
    /* The following statement has a buffer overflow problem */
    fread(buffer, sizeof(char), 76,  badfile);
    return 1;
}


int main(int argc, char **argv)
{
    FILE *badfile;
    badfile = fopen("badfile", "r");
    bof(badfile);
    printf("Returned Properly\n");
    fclose(badfile);
    return 1;
}
