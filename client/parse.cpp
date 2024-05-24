#include <stdlib.h>
#include <string.h>

#include "parse.h"

static unsigned ifsrun(const char* input, const char* ift) {
	unsigned i;
	for (i = 0; input[i] != '\0' && strchr(ift, input[i]); i++);
	return i;
}

int strtodata(const char* str, char** data, size_t* size) {
	const char* ifs = IFS;
	*size = 0;
	size_t i;
	size_t str_len = strlen(str);

	// TODO: finish this parser
	// for (i = 0; i < str_len; i++) {
	// 	unsigned ifs_run = ifsrun(&str[i], ifs);
	// 	
	// }

	*size = str_len;

	*data = (char*) malloc(*size);

	memcpy(*data, str, *size);

	return 0;
}

