#include "strings.h"

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    if(result==NULL) {
        perror("Out of memory");
        exit(-1);
    }
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

char* readFromFile(const char* fname) {
    char* buffer;
    long length;
    FILE* f = fopen(fname, "rb");
    if(f) {
        fseek(f,0,SEEK_END);
        length = ftell(f);
        fseek(f,0,SEEK_SET);
        buffer=malloc(length);
        if(buffer) {
            size_t ret = fread(buffer,1,length,f);
        }
        fclose(f);
    }
    return buffer;
}