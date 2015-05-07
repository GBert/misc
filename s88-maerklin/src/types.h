// -----------------------------------------------------------------------------
// $RCSfile: types.h,v $
// Project: Maerklin Central Station II
// Subproject: S88 Driver
// Date of Creation: 17.07.2007
// Main Author: C.Biermann
// Copyright: (c) 2007 ies GmbH, 58566 Kierspe, Germany
//            All Rights Reserved.
//
// $Revision: 1.2 $
// -----------------------------------------------------------------------------
// Description:
//! \file  types.h
//! \brief common type declarations
//!
//! ...detailed description...
//!
// -----------------------------------------------------------------------------
// Optional Information, e.g. Module List
//
//
// -----------------------------------------------------------------------------
// $Log: types.h,v $
// Revision 1.2  2008/07/22 09:22:04  bie
// Release of driver version 1.0.0
//
// Revision 1.1  2007/12/28 14:29:53  bie
// erste komplett lauffaehige Version
//
// -----------------------------------------------------------------------------

#ifndef _TYPES_H
#define _TYPES_H


#define MAXBYTE   255
#define MAXWORD   65535
#define MAXSHORT  32767

#define false 0
#define true  1

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned long int DWORD;
typedef signed char CHAR;
typedef signed short int SHORT;
typedef signed long int LONG;
typedef BYTE BOOL;


#endif
