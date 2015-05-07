// -----------------------------------------------------------------------------
// $RCSfile: s88.h,v $
// Project: Maerklin CST2
// Subproject: Device Driver for S88 I/O Modules
// Date of Creation: 13.09.2007
// Main Author: C.Biermann
// Copyright: (c) 2007 ies GmbH, 58566 Kierspe, Germany
//            All Rights Reserved.
//
// $Revision: 1.6 $
// -----------------------------------------------------------------------------
// Description:
//! \file  s88.h
//! \brief This file contains globally used declarations that must be included
//!        by applications using this driver
//!
//!
// -----------------------------------------------------------------------------
// Optional Information, e.g. Module List
//
//
// -----------------------------------------------------------------------------
// $Log: s88.h,v $
// Revision 1.6  2008/07/22 09:22:04  bie
// Release of driver version 1.0.0
//
// Revision 1.5  2008/07/14 13:14:06  bie
// extended maximal length of s88 bus from 496 to 1024 inputs;
//
// Revision 1.4  2008/06/25 14:28:31  bie
// use periodic mode of operating system timer instead of one-shot mode again;
//
// Revision 1.3  2008/06/17 13:16:31  bie
// moved falling edge of s88 shift clock towards end of shift cycle;
//
// Revision 1.2  2008/02/12 08:57:43  bie
// Error correction: Inputs 9-16 of a module are now read correctly;
// now using dynamic major device numbers during device registration;
//
// Revision 1.1  2007/12/28 14:30:13  bie
// erste komplett lauffaehige Version
//
// -----------------------------------------------------------------------------


#ifndef _S88_H
#define _S88_H

#define S88_VERSION             "1.0.0"


/**
 * The S88 bus length must be within the following range.
 * The length is specified in Bits, one module has 16 bits.
 */

#define S88_MIN_BUS_LENGTH      0
#define S88_MAX_BUS_LENGTH      1024
#define S88_DFL_BUS_LENGTH      0


/**
 * The cycle time for scanning all modules must be within the following range.
 * All values in milliseconds.
 */

#define S88_MIN_CYCLE_TIME      10           // 100 Hz
#define S88_MAX_CYCLE_TIME      10000        // 0,1 Hz
#define S88_DFL_CYCLE_TIME      100          // 10 Hz


/**
 * The cycle time of the shift clock must be within the following range.
 * All values in microseconds.
 */

#define S88_MIN_SHIFT_CLOCK     100          // 10 KHz
#define S88_MAX_SHIFT_CLOCK     100000       // 10 Hz
#define S88_DFL_SHIFT_CLOCK     167          // 6 KHz

#define S88_SHIFT_CLOCK_PAUSE   30           // low period of shift clock


/**
 * The driver can operate in one of two operating modes:
 * Mode S88_MODE_ABSOLUTE returns the actual status of all read inputs,
 * mode S88_MODE_CHANGES returns inputs that have changed their status since the
 * last read operation.
 */

#define S88_MODE_ABSOLUTE       0
#define S88_MODE_CHANGES        1
#define S88_LAST_MODE           S88_MODE_CHANGES


/**
 * iotctl definitions
 */

#define S88_IOC_MAGIC           'S'

#define S88_IOC_SET_CYCLE       _IOW(S88_IOC_MAGIC, 1, int)
#define S88_IOC_SET_CLOCK       _IOW(S88_IOC_MAGIC, 2, int)
#define S88_IOC_SET_BUS_LENGTH  _IOW(S88_IOC_MAGIC, 3, int)
#define S88_IOC_SET_MODE        _IOW(S88_IOC_MAGIC, 4, int)

#define S88_IOC_MAX_NUMBER      _IOC_NR(S88_IOC_SET_MODE)


#endif
