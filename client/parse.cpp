#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parse.h"

static int parse_str(const char* str, char* data, size_t* size) {
	char closing = str[0];
	char escape = false;
	bool scan = data == NULL;
	int i = 0;
	size_t len = strlen(str);

	switch (str[i]) {
		case '\'':
			escape = false;
			break;
		case '\"':
			escape = true;
			break;
		default:
			return -i;
	}

	for (i = 1; i < len && str[i] != '\0'; i++) {
		char c = str[i];

		if (c == closing) {
			if (scan) printf("string%s of length %d\n", escape ? " (w/ escape)" : "", i - 1);
			return i + 1; // +1 for closing quote
		}

		if (scan) *size += 1;
	}

	return -i;
}

static int parse_num(const char* str, char* data, size_t* size) {
	const char* ifs = IFS;
	size_t len = strcspn(str, ifs);
	bool scan = data == NULL;
	int i = 0;
	int base = 10;
	bool bytestring = false;

	const char* colon = strchr(str, ':');
	if (colon != NULL && colon < str + len) { // byte string
		base = 16;
		bytestring = true;
	} else if (len > 2 && strncmp(str, "0x", 2) == 0) { // hexadecimal prefix
		base = 16;
		i += 2;
	}/* else if (len > 1 && strncmp(str, "0", 1) == 0) { // octal prefix
		base = 8;
		i += 1;
	}*/

	const char* set;
	// if (base == 8) set = SET_OCT;
	if (base == 10) set = SET_DEC;
	if (base == 16) {
		if (bytestring) set = SET_HEX_STR;
		else set = SET_HEX;
	}
	
	size_t len_ok = strspn(str + i, set) + i;
	if (len != len_ok) return -len_ok;

	if (scan) {
		if (base == 10) *size += 1;
		else if (base == 16) {
			if (!bytestring) {
				*size += (len - i + 1) / 2;
			} else {
				for (; colon != NULL && colon < str + len; colon = strchr(str, ':')) {
					*size += 1;
				}
			}
		}
	}

	if (scan) printf("number (base %d%s) of length %lu\n", base, bytestring ? " as bytestring" : "", len - i);
	return len;
}

int strtodata(const char* str, char** data, size_t* size) {
	const char* ifs = IFS;
	*size = 0;
	size_t i;
	size_t len = strlen(str);

	for (i = 0; i < len;) {
		// skip whitespace
		int run;
		run = strspn(&str[i], ifs);
		if (run > 0) printf("skipping whitespace for %d bytes...\n", run);
		i += run;
		// end of string
		if (str[i] == '\0') break;

		if ((run = parse_str(str + i, NULL, size)) > 0) { i += run; continue; }
		if ((run = parse_num(str + i, NULL, size)) > 0) { i += run; continue; }

		// no format detected
		return -i + run;
	}
	printf("end of string w/o parse errors\n");
	printf("buffer size is now %lu\n", *size);
	exit(0);

	*data = (char*) malloc(*size);

	return 0;
}

