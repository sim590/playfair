#include <string.h>

#include "shelper.h"

#define MIN_ASCII_CAPLETTER_VALUE 65
#define MAX_ASCII_CAPLETTER_VALUE 90
#define MIN_ASCII_LETTER_VALUE 97
#define MAX_ASCII_LETTER_VALUE 122

bool is_letter(char c) {
    return (MIN_ASCII_LETTER_VALUE < c && c < MAX_ASCII_LETTER_VALUE)
        || (MIN_ASCII_CAPLETTER_VALUE < c && c < MAX_ASCII_CAPLETTER_VALUE);
}

char* substr(char* str, size_t begin, size_t end) {
    size_t slen = end-begin+1;
    if (slen <= 0) return NULL;

    char* ss = (char*)calloc(slen, sizeof(char));
    memcpy(ss, str+begin, slen);
    ss[slen] = '\0';
    return ss;
}

bool is_in_str(char c, char* str, size_t begin, int end) {
    for (size_t i = begin; i < end; ++i)
        if (str[i] == c) return true;
    return false;
}

/* vim: set sts=4 ts=4 sw=4 tw=120 et :*/

