#pragma once

#include <stddef.h>

#define IFS " \t\n"

#define SET_OCT "01234567"
#define SET_DEC "0123456789"
#define SET_HEX SET_DEC"abcdefABCDEF"
#define SET_HEX_STR SET_HEX":"

/**
 * \brief modify \p token to point to the first token when broken up on \p ifs
 * and return the remaining data
 *
 * \p token will be null-terminated to indicate the end of the first token. A
 * pointer to the remaining line after the NULL byte is returned, or NULL when
 * the end of the string has been reached.
 *
 * \param token  input string
 * \param ifs    string containing field separators
 *
 * \return the remaining data after \p token and the first \p ifs
 */
char* consume_token(char* token, const char* ifs);

/**
 * \brief convert string with literals into raw data
 *
 * \param str   input string containing literals
 * \param data  pointer to \c char* that will store the resulting data
 * \param size  size of \p data
 *
 * \return 0 or a negative integer representing the index where there is a
 * syntax error if there was an error, or a positive integer representing the
 * amount of bytes parsed from \p str
 *
 * \note The pointer that \p data refers to will not be initialized by this
 * function if parsing fails
 */
int strtodata(const char* str, char** data, size_t* size);

