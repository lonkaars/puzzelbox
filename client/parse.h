#pragma once

#include <stddef.h>

#define IFS " \t\n"

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
 * \return 0 if the string was parsed succesfully, or 1 if the string could not
 * be parsed succesfully
 *
 * \note The pointer that \p data refers to will not be initialized by this
 * function if parsing fails
 * \note \p size will contain the index of \p str where the first invalid data
 * was found if parsing fails
 */
int strtodata(const char* str, char** data, size_t* size);

