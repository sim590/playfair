#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "config.h"

#define BUFLEN 1024
#define SELCHAR 'i'

#define MIN_ASCII_CAPLETTER_VALUE 65
#define MAX_ASCII_CAPLETTER_VALUE 90
#define MIN_ASCII_LETTER_VALUE 97
#define MAX_ASCII_LETTER_VALUE 122

#define WHITESPACES " \f\n\r\t\v"

char alphabet[25] = {
    'a', 'b', 'c',     'd', 'e',
    'f', 'g', 'h', SELCHAR, 'k',
    'l', 'm', 'n',     'o', 'p',
    'q', 'r', 's',     't', 'u',
    'v', 'w', 'x',     'y', 'z'
};

char key[25];
char BUF[BUFLEN];

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

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    char* subkey = argv[1];
    fill_key(subkey);

#if DEBUG==1
    for (size_t i = 0; i < 25; ++i) {
        printf("%c ", key[i]);
        if (i%5 == 4)
            printf("\n");
    }
    printf("\n");
#endif

    char* plaintext = malloc(sizeof(char));
    plaintext[0] = '\0';
    while (fgets(BUF, BUFLEN, stdin)) {
        plaintext = realloc(plaintext, BUFLEN);
        strcpy(plaintext, BUF);
    }

#if DEBUG==1
    printf("Texte clair:\n");
    printf("%s\n\n", plaintext);
#endif

    convert_for_scheme_input(&plaintext);

#if DEBUG==1
    printf("Texte clair converti:\n");
    printf("%s\n\n", plaintext);
#endif

    char* ciphertext = encrypt(plaintext);

    printf("%s\n", ciphertext);

#if DEBUG==1
    char* decryptedtext = decrypt(ciphertext);
    printf("Texte déchiffré:\n");
    printf("%s\n\n", decryptedtext);
#endif
    return 0;
}

/* vim: set sts=4 ts=4 sw=4 tw=120 et :*/

