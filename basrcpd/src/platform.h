// platform.h : PI platform access 
//
// C 2018 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#ifndef _PLATFORM_PI_H
#define _PLATFORM_PI_H

#include <stdint.h>


typedef enum PLFUNCTION {
    PL_SERIAL = 0,
    PL_TEMP
} pl_function_t;

uint64_t getPlatformData(pl_function_t fucode);


#endif	// _PLATFORM_PI_H
