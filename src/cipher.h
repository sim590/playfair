#ifndef CIPHER_H_PYZ8REGC
#define CIPHER_H_PYZ8REGC

const char* getkey();

void fill_key(const char* subkey);
void convert_for_scheme_input(char** strp);
char* encrypt(const char* plaintext);
char* decrypt(const char* ciphertext);

#endif /* end of include guard: CIPHER_H_PYZ8REGC */

/* vim: set sts=4 ts=4 sw=4 tw=120 et :*/

