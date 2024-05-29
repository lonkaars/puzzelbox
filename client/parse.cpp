#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

#include "parse.h"

static int parse_string(const char * str, char * data, size_t * offset) {
	char escape = false;
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
	char closing = str[i];

	for (i = 1; i < len && str[i] != '\0'; i++, *offset += 1) {
		char c = str[i];

		if (c == closing)
			return i + 1; // +1 for closing quote

		if (escape && c == '\\') {
			char x = str[i + 1];
			if      (x == '0')  c = '\0';
			else if (x == 't')  c = '\t';
			else if (x == 'n')  c = '\n';
			else if (x == 'r')  c = '\r';
			else if (x == '\\') c = '\\';
			else if (x == '\"') c = '\"';
			else if (x == '\'') c = '\'';
			else break;
			i++;
		}

		if (data != NULL)
			data[*offset] = c;
	}

	return -i;
}

static int parse_hexstr(const char * str, char * data, size_t * offset) {
	size_t len = strcspn(str, IFS);
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

		if (data != NULL)
			data[*offset] = strtol(str + c, NULL, 16) & 0xff;

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

static int parse_number(const char * str, char * data, size_t * offset) {
	size_t len = strcspn(str, IFS);
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

	size_t size = 1; // default integer size in bytes
	if (base == 16) {
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
		size = prefixless / 2;
	}

	if (data != NULL) {
		unsigned long number = strtol(str + i, NULL, base);
		long long mask = (1 << 8 * size) - 1;
		number &= mask;
		// NOTE: the hton? functions are used to convert host endianness to network
		// endianness (big), and are required
		switch (size) {
			case 1:
				data[*offset] = number & 0xff;
				break;
			case 2:
				number = htons(number);
				// TODO: check if the endianness is OK, or reverse these *offset indices*
				data[*offset + 1] = (number)       & 0xff;
				data[*offset + 0] = (number >>= 8) & 0xff;
				break;
			case 4:
				number = htonl(number);
				data[*offset + 3] = (number)       & 0xff;
				data[*offset + 2] = (number >>= 8) & 0xff;
				data[*offset + 1] = (number >>= 8) & 0xff;
				data[*offset + 0] = (number >>= 8) & 0xff;
				break;
		}
	}

	*offset += size;
	return len;
}

static int _strtodata_main(const char * str, char* data, size_t * offset) {
	size_t len = strlen(str);

	int i, run;
	for (i = 0; i < len; i += run) {
		i += strspn(&str[i], IFS); // skip whitespace
		if (str[i] == '\0') break; // end of string

		if ((run = parse_string(str + i, data, offset)) > 0) continue;
		if ((run = parse_hexstr(str + i, data, offset)) > 0) continue;
		if ((run = parse_number(str + i, data, offset)) > 0) continue;

		// no format detected
		return -i + run;
	}

	return i;
}

int strtodata(const char * str, char ** data, size_t * size) {
	*size = 0;

	// 1st pass: check data format
	int ret = _strtodata_main(str, NULL, size);
	if (ret <= 0) return ret; // on error

	// 2nd pass: convert string literals into binary data
	*data = (char*) malloc(*size);
	size_t written = 0;
	return _strtodata_main(str, *data, &written);
}

