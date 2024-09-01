#include <assert.h>
#include <stdlib.h>
#include <string.h>

void *checked_malloc(int length)
{
    void *p = malloc(length);
    assert(p);
    return p;
}

typedef char *string;

string create_string(char* value) {
    int length = strlen(value);
    string result = checked_malloc(length * sizeof(char));
    strcpy(result, value);
    return result;
}

typedef char bool;
#define TRUE 1;
#define FALSE 0;
