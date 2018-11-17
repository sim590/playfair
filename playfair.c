#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "cipher.h"

#define BUFLEN 1024

char BUF[BUFLEN];

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    char* subkey = argv[1];
    fill_key(subkey);

#if DEBUG==1
    const char* key = getkey();
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

