//                            Software License Agreement
//
// By Jeff Post, j_post <AT> pacbell <DOT> net
//
// You may use, copy, modify and distribute the Software for use with Microchip
// products only.  If you distribute the Software or its derivatives, the
// Software must have this entire copyright and disclaimer notice prominently
// posted in a location where end users will see it (e.g., installation program,
// program headers, About Box, etc.).  To the maximum extent permitted by law,
// this Software is distributed “AS IS” and WITHOUT ANY WARRANTY INCLUDING BUT
// NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR
// PARTICULAR PURPOSE, or NON-INFRINGEMENT. IN NO EVENT WILL MICROCHIP OR ITS
// LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
// DAMAGES OF ANY KIND ARISING FROM OR RELATED TO THE USE, MODIFICATION OR
// DISTRIBUTION OF THIS SOFTWARE OR ITS DERIVATIVES.
//
//---------------------------------------------------------------------------

// stdafx.cpp : source file that includes just the standard includes
// pk2cmd.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include	<stdarg.h>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


#ifndef WIN32

char *_tcsncpy_s(char *dst, const char *src, int len)
{
	int	i;

	for (i=0; i<len; i++)
		dst[i] = src[i];

	dst[len] = 0;
	return dst;
}

/* Called from cmd_app.cpp

	_tsearchenv_s("PK2DeviceFile.dat", "PATH", tempString);
*/

void _tsearchenv_s(const char *fname, const char *path, char *bfr)
{
	int	offset = 0;//, found = 0;
	char	*env;
	char	file[MAX_PATH];

	env = getenv(path);

	if (env)
	{
		while (*env && (*env != ':'))
		{
			file[offset++] = *env++;

			if ((!*env || *env == ':') && offset)
			{
				file[offset] = '\0';
				offset = 0;

				if (*env)
					env++;

				strcat(file, "/");
				strcat(file, fname);

				if (!access(file, 0))
				{
					strcpy(bfr, file);
					break;
				}

			}
		}
	}
}

void _localtime64_s(struct tm *x, time_t *y)
{
	struct tm	*ltime;

	ltime = localtime(y);
	memcpy(x, ltime, sizeof(struct tm));
}

/* The only call to _tcsftime is in cmd_app.cpp:

	time_t now;
	struct tm today;
	char stime[128] = "";

	_tzset();
	time(&now);
   _localtime64_s(&today, &now);
	_tcsftime( stime, 128, "%d-%b-%Y, %H:%M:%S", &today );
	                        This format is some non-standard Micros$$t junk
*/

void _tcsftime(char *bfr, int len, const char *fmt, struct tm *time)
{
	sprintf(bfr, "%d-%d-%d, %d:%02d:%02d", time->tm_mday, time->tm_mon + 1, time->tm_year + 1900,
		time->tm_hour, time->tm_min, time->tm_sec);
}

void _stprintf_s(char *bfr, int size, const char *fmt, ...)
{
	va_list	argp;
	va_start(argp, fmt);
	vsprintf(bfr, fmt, argp);
	va_end(argp);
}

//	if ((err = fopen_s(&datfile, path, "rb")) == 0)

int fopen_s(FILE **fp, char *path, const char *spec)
{
	FILE	*fptr;

	errno = 0;
	*fp = NULL;
	fptr = fopen(path, spec);
	*fp = fptr;
	return errno;
}

#endif
