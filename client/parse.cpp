#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parse.h"

static int parse_string(const char* str, char* data, size_t* offset) {
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

		// TODO: handle escaped characters

		if (c == closing)
			return i + 1; // +1 for closing quote

		*offset += 1;
	}

	return -i;
}

static int parse_hexstr(const char* str, char* data, size_t* offset) {
	const char* ifs = IFS;
	size_t len = strcspn(str, ifs);
	bool scan = data == NULL;
	int i = 0;

	// check if token contains at least one colon
	const char* colon = strchr(str, ':');
	if (colon == NULL) return -i;
	if (colon >= str + len) return -i;

	// check if token only contains allowed characters [0-9a-fA-F:]
	size_t len_ok = strspn(str + i, SET_HEX_STR) + i;
	if (len != len_ok) return -len_ok;

	size_t c = 0;
	while (c < len) { // count bytes in bytestring
		if (strspn(str + c, SET_HEX) != 2)
			return -i -c;
		c += 2;
		*offset += 1;

		if (str[c] == ':') {
			c += 1;
			continue;
		}
		break;
	}

	i += len;
	return i;
}

static int parse_number(const char* str, char* data, size_t* offset) {
	const char* ifs = IFS;
	size_t len = strcspn(str, ifs);
	bool scan = data == NULL;
	int i = 0;
	int base = 10;
	bool bytestring = false;

	if (len > 2 && strncmp(str, "0x", 2) == 0) { // hexadecimal prefix
		base = 16;
		i += 2;
	}/* else if (len > 1 && strncmp(str, "0", 1) == 0) { // octal prefix
		base = 8;
		i += 1;
	}*/

	const char* set;
	// if (base == 8) set = SET_OCT;
	if (base == 10) set = SET_DEC;
	if (base == 16) set = SET_HEX;
	
	size_t len_ok = strspn(str + i, set) + i;
	if (len != len_ok) return -len_ok;

	if (base == 10) *offset += 1;
	else if (base == 16) {
		size_t prefixless = len - i;
		switch (prefixless) {
			case 2:  //  8-bit (2 hex characters)
			case 4:  // 16-bit
			case 8:  // 32-bit
			case 16: // 64-bit
				break;
			default:
				return -i;
		}
		*offset += prefixless / 2;
	}

	i += len;
	return i;
}

static int _strtodata_main(const char* str, char* _data, size_t* offset) {
	const char* ifs = IFS;
	size_t len = strlen(str);

	size_t i = 0;

	while (i < len) {
		i += strspn(&str[i], ifs); // skip whitespace
		if (str[i] == '\0') break; // end of string

		int run;
		char* data = _data == NULL ? NULL : _data + *offset;
		if ((run = parse_string(str + i, data, offset)) > 0) goto format_ok;
		if ((run = parse_hexstr(str + i, data, offset)) > 0) goto format_ok;
		if ((run = parse_number(str + i, data, offset)) > 0) goto format_ok;

		return -i + run; // no format detected

format_ok:
		i += run;
		continue;
	}

	return i;
}

int strtodata(const char* str, char** data, size_t* size) {
	*size = 0;

	// 1st pass: check data format
	int ret = _strtodata_main(str, NULL, size);
	if (ret <= 0) return ret; // on error

	// 2nd pass: convert string literals into binary data
	*data = (char*) malloc(*size);
	size_t written = 0;
	return _strtodata_main(str, *data, &written);
}

