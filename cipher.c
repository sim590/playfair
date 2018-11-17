#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "cipher.h"
#include "shelper.h"

#define SELCHAR 'i'

char alphabet[25] = {
    'a', 'b', 'c',     'd', 'e',
    'f', 'g', 'h', SELCHAR, 'k',
    'l', 'm', 'n',     'o', 'p',
    'q', 'r', 's',     't', 'u',
    'v', 'w', 'x',     'y', 'z'
};

char key[25];

const char* getkey() {return key;}

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

void convert_for_scheme_input(char** strp) {
    char* str = *strp;
    size_t slen = strlen(str);
    size_t ssize = slen+1;
    for (size_t i = 0; i < slen; i+=2) {
        i += strspn(str+i, WHITESPACES);
        size_t j = i+1;
        j += strspn(str+j, WHITESPACES);
        str[i] = tolower(str[i]);
        str[j] = tolower(str[j]);
        if (str[i] == str[j] || !is_letter(str[j])) {
            if (strlen(str)+1 == ssize) {
                ssize *= 2;
                str = realloc(str, ssize);
            }
            char* rp = substr(str, i+1, strlen(str));
            str[i+1] = 'x'; str[i+2] = '\0';
            if (rp)
                strcat(str, rp);
            free(rp);
            ++slen;
        }
        i = j-1;
    }
    str = realloc(str, slen);
}

char* transform_text(const char* text, const char* op) {
    int dir = !strcmp(op, "encrypt") ? 1 : (!strcmp(op, "decrypt") ? -1 : 0);
    if (!dir) return NULL;

    size_t plen = strlen(text);
    char* rtext = (char*)calloc(plen+1, sizeof(char));
    strcpy(rtext, text);
    for (size_t i = 0; i < plen; i+=2) {
        i += strspn(text+i, WHITESPACES);
        size_t j = i+1;
        j += strspn(text+j, WHITESPACES);
        int ki = strchr(key, text[i])-key;
        int kj = strchr(key, text[j])-key;
        if (ki/5 == kj/5) { /* sur la même ligne ! */
            rtext[i] = key[(ki/5)*5+(ki+dir)%5];
            rtext[j] = key[(kj/5)*5+(kj+dir)%5];
        } else if (ki%5 == kj%5) { /* sur la même colonne ! */
            rtext[i] = key[((ki+dir*5)%25+25)%25];
            rtext[j] = key[((kj+dir*5)%25+25)%25];
        } else { /* dans un rectangle ! */
            rtext[i] = key[(ki/5)*5+kj%5];
            rtext[j] = key[(kj/5)*5+ki%5];
        }
        i = j-1;
    }
    return rtext;
}
char* encrypt(const char* plaintext)  { return transform_text(plaintext, "encrypt");  }
char* decrypt(const char* ciphertext) { return transform_text(ciphertext, "decrypt"); }

