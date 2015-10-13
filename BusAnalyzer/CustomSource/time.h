/******************************************************************************
 * @file time.h
 *
 * @brief This module handles any time specifics. For details refer to the
 *        modules implementation.
 *
 * @author Alex
 *****************************************************************************/

#ifndef TIME_H_
#define TIME_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <inttypes.h>

/******************************************************************************
 * Function declarations
 *****************************************************************************/

void Time_initialize(void);
void Time_reset(void);
uint64_t Time_getUs(void);

#endif /* TIME_H_ */
