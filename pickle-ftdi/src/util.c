/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 * This file is part of Pickle Microchip PIC ICSP.
 * 
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "pickle.h"

/******************************************************************************
 * UTILITY FUNCTIONS
 *****************************************************************************/

/*
 * are you sure?
 */
int
areyousure(const char *s)
{
	int c;

	printf("%s: are you sure [y/N]? ", s);
	c = fgetc(stdin);
	if (c == 'y' || c == 'Y')
		return 1;

	return 0;
}

/*
 * strcasestr implementation for cygwin
 */
const char *
mystrcasestr(const char *haystack, const char *needle)
{
	int i, j;
	int nlen = strlen(needle);
	int hlen = strlen(haystack) - nlen;

	for (i = 0; i <= hlen; i++) {
		for (j = 0; j < nlen; j++) {
			if (tolower((int)haystack[i + j]) != tolower((int)needle[j]))
				break;
		}
		if (j == nlen) {
			return &haystack[i];
		}
	}
	return NULL;
}

/*
 * return the value of a hex nibble
 */
uint8_t
hex2nibble(const char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

/*
 * return the value of a hex byte
 */
uint8_t
hex2byte(const char *s)
{
	if (s[0] && s[1])
		return hex2nibble(s[0]) << 4 | hex2nibble(s[1]);
	return 0;
}

/*
 * return string with CRLF removed from end
 */
char *
rmcrlf(char *s, int slen)
{
	int l;

	s[slen - 1] = '\0';

	l = strlen(s) - 1;

	while (l >= 0 && (s[l]=='\r' || s[l]=='\n'))
		s[l--]= '\0';

	return s;
}
