
#ifndef SHELPER_H_XKMGGPQR
#define SHELPER_H_XKMGGPQR

#include <stdbool.h>
#include <stdlib.h>

#define WHITESPACES " \f\n\r\t\v"

bool is_letter(char c);
char* substr(char* str, size_t begin, size_t end);
bool is_in_str(char c, char* str, size_t begin, int end);

#endif /* end of include guard: SHELPER_H_XKMGGPQR */

/* vim: set sts=4 ts=4 sw=4 tw=120 et :*/

