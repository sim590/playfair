#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

#define BUFLEN 1024
#define SELCHAR 'i'

char alphabet[25] = {
    'a', 'b', 'c',     'd', 'e',
    'f', 'g', 'h', SELCHAR, 'k',
    'l', 'm', 'n',     'o', 'p',
    'q', 'r', 's',     't', 'u',
    'v', 'w', 'x',     'y', 'z'
};

char key[25];
char BUF[BUFLEN];

bool is_in_str(char c, char* str, size_t begin, int end) {
    for (size_t i = begin; i < end; ++i)
        if (str[i] == c) return true;
    return false;
}

size_t fill_from_str(const char* str, const char* execpt, size_t begin) {
    size_t slen = strlen(str);
    size_t kp, sp = 0;
    for (kp = begin; kp < 25; ++kp, ++sp) {
        while(sp < slen && is_in_str(*(str+sp), key, 0, 25)) {
            ++sp;
        }
        if (sp >= slen)
            return kp-begin;
        else
            key[kp] = *(str+sp);
    }
    return kp-begin;
}

void fill_key(const char* subkey) {
    size_t cnt = fill_from_str(subkey, NULL, 0);
    fill_from_str(alphabet, subkey, cnt);
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    char* subkey = argv[1];
    fill_key(subkey);

    return 0;
}

/* vim: set sts=4 ts=4 sw=4 tw=120 et :*/

