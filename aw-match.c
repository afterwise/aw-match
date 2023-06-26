
/*
   Copyright (c) 2014-2023 Malte Hildingsson, malte (at) afterwi.se

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
 */

#if !defined(_MSC_VER) || _MSC_VER >= 1800
# include <stdbool.h>
#endif
#include "aw-match.h"
#include "aw-utf8.h"

/*
   Inspired by Rob Pike's regex implementation in The Practice of Programming:
   http://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
 */

#define NEGATE (0x1)
#define CLASS (0x2)

static bool match_here(struct utf8_iter expr, struct utf8_iter text, char **end);

static bool match_char(unsigned c, int flags, struct utf8_iter *text) {
	bool yes = !(flags & NEGATE);

	if (!(flags & CLASS)) {
		if (c == text->chr)
			return yes;
	} else if (c == 'a') {
		if ((text->chr >= 0x41 && text->chr <= 0x5a) ||
				(text->chr >= 0x61 && text->chr <= 0x7a) ||
				(text->chr >= 0xc0 && text->chr <= 0xd6) ||
				(text->chr >= 0xd8 && text->chr <= 0xf6) ||
				(text->chr >= 0xf8 && text->chr <= 0xff))
			return yes;
	} else if (c == 'd') {
		if (text->chr >= 0x30 && text->chr <= 0x39)
			return yes;
	} else if (c == 'f') {
		if ((text->chr >= 0x30 && text->chr <= 0x39) || text->chr == 0x2e)
			return yes;
	} else if (c == 'g') {
		if (text->chr > 0x20)
			return yes;
	} else if (c == 'n') {
		if (text->chr == 0xa || text->chr == 0xd)
			return yes;
	} else if (c == 's') {
		if (text->chr == 0x9 || text->chr == 0xa || text->chr == 0xd || text->chr == 0x20)
			return yes;
	} else if (c == 'x') {
		if ((text->chr >= 0x41 && text->chr <= 0x46) ||
				(text->chr >= 0x61 && text->chr <= 0x66) ||
				(text->chr >= 0x30 && text->chr <= 0x39))
			return yes;
	} else if (c == '?') {
		return yes;
	}

	return !yes;
}

static bool match_star(unsigned c, int flags, struct utf8_iter expr, struct utf8_iter text, char **end) {
	for (; !match_here(expr, text, end); utf8_next(&text))
		if (text.chr == 0 || !match_char(c, flags, &text))
			return false;

	return true;
}

static bool match_plus(unsigned c, int flags, struct utf8_iter expr, struct utf8_iter text, char **end) {
	if (!match_char(c, flags, &text))
		return false;

	do utf8_next(&text);
	while (text.chr != 0 && match_char(c, flags, &text));

	return match_here(expr, text, end);
}

static bool match_here(struct utf8_iter expr, struct utf8_iter text, char **end) {
	unsigned pre = expr.chr;
	int flags = 0;

	if (pre == 0) {
		if (end != NULL)
			*end = text.head;

		return true;
	}

	if (pre == '~') {
		pre = utf8_next(&expr);
		flags |= NEGATE;
	}

	if (pre == '%') {
		pre = utf8_next(&expr);

		switch (pre) {
		case 'a': /* alpha */
		case 'd': /* digit */
		case 'f': /* float */
		case 'g': /* graphic */
		case 'n': /* newline */
		case 's': /* space */
		case 'x': /* hex */
		case '?': /* anything */
			flags |= CLASS;
		}
	}

	utf8_next(&expr);

	if (expr.chr == '*') {
		utf8_next(&expr);
		return match_star(pre, flags, expr, text, end);
	} else if (expr.chr == '+') {
		utf8_next(&expr);
		return match_plus(pre, flags, expr, text, end);
	}

	if (pre == '$' && expr.chr == 0) {
		if (end != NULL)
			*end = text.head;

		return text.chr == 0;
	}

	if (text.chr != 0 && match_char(pre, flags, &text)) {
		utf8_next(&text);
		return match_here(expr, text, end);
	}

	return false;
}

char *match(char *expr_, char *text_, char **end) {
	struct utf8_iter expr;
	struct utf8_iter text;

	utf8_iter(&expr, expr_);
	utf8_iter(&text, text_);

	utf8_next(&text);

	if (utf8_next(&expr) == '^') {
		utf8_next(&expr);
		return match_here(expr, text, end) ? text.head : NULL;
	}

	do if (match_here(expr, text, end))
		return text.head;
	while (text.chr != 0 && utf8_next(&text));

	return NULL;
}

