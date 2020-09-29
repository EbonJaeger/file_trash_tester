#include "utils.h"

char *substring(char *source, char *dest, int offset, int length)
{
    int input_length = strlen(source);
    if (offset + length > input_length)
    {
        return '\0';
    }

    strncpy(dest, source + offset, length);
    return dest;
}
