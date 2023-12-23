
#include <assert.h>
#include <stdio.h>
#include "aw-match.h"

int main(int argc, char *argv[]) {
	char *expr;
	char *str;
	char *end;
	char *res;

	(void) argc;
	(void) argv;

	expr = "aaa";
	str = "aa";
	res = match(expr, 0, str, &end);
	assert(res == NULL);

	expr = "aaa";
	str = "xaa";
	res = match(expr, 0, str, &end);
	assert(res == NULL);

	expr = "aaa";
	str = "aax";
	res = match(expr, 0, str, &end);
	assert(res == NULL);

	expr = "aaa";
	str = "aaa";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "a%?a";
	str = "aaa";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "aax*a";
	str = "aaa";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "aa%?*a";
	str = "aaa";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "aa*a";
	str = "aaa";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 'a');

	expr = "^aa*a$";
	str = "aaa";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "aaa";
	str = "xaaa";
	res = match(expr, 0, str, &end);
	assert(res == str + 1);
	assert(*end == 0);

	expr = "aaa";
	str = "aaax";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 'x');

	expr = "^aaa$";
	str = "aaa";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "aaa$";
	str = "xaaa";
	res = match(expr, 0, str, &end);
	assert(res == str + 1);
	assert(*end == 0);

	expr = "^aaa";
	str = "aaax";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 'x');

	expr = "^%a%a*$";
	str = "Ïåaäêø";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "~%g";
	str = "\f";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "%a%d*%a%d";
	str = "xyzw a12b3\n";
	res = match(expr, 0, str, &end);
	assert(res == str + 5);
	assert(*end == '\n');

	expr = "%f%f*%s~%s%?*\n";
	str = "1024.0123 X\r\n";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 0);

	expr = "%x+";
	str = "0123456789ABCdefG";
	res = match(expr, 0, str, &end);
	assert(res == str);
	assert(*end == 'G');

	printf("OK!\n");
	return 0;
}

