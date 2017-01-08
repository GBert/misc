/*
 * Copyright (C) 2005-2017 Darron Broad
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

#ifndef _PIC14_H
#define _PIC14_H

/******************************************************************************
 * 14-bit CONFIGURATION
 *****************************************************************************/

#define PIC14_MASK (0x3FFF)
#define PIC14_CONFIG_MAX (4)
#define PIC14_CALIB_MAX  (32)
#define PIC14_USERID_MAX (4)

struct pic14_config {
	uint16_t userid[PIC14_USERID_MAX];	/* X000 .. X003  */
	uint16_t reserved;			/* X004          */
	uint16_t revisionid;			/* X005          */
	uint16_t deviceid;			/* X006          */
	uint16_t config[PIC14_CONFIG_MAX];	/* X007 .. X009  */
	uint16_t calib[PIC14_CALIB_MAX];	/* X00X .. X0XX  */
	uint16_t osccal;			/* 03FF DS41191C */
};

struct pic14_dsmap {
	char devicename[STRLEN];/*                                                     */
	uint16_t deviceid;	/*                                                     */
	uint16_t flash;		/* program flash size in words                         */
	uint16_t eeprom;	/* data eeprom size in bytes 0=none                    */
	uint32_t datasheet;	/*                                                     */
	uint16_t configaddr;	/* CONFIG BASE                                         */
	uint16_t dataaddr;	/* DATA BASE                                           */
	uint16_t calibaddr;	/* CALIB BASE                                          */
	uint8_t nconfig;	/* NUMBER OF CONFIG WORDS AT CONFIG BASE + 7           */
	uint8_t ncalib;		/* NUMBER OF CALIB. WORDS AT CONFIG BASE + 7 + nconfig */
	uint8_t nlatches;	/* PROGRAM FLASH MULTI-WORD LATCHES                    */
	uint8_t erasesize;	/* ERASE BLOCK SIZE IF ROW ERASE SUPPORTED             */
#if 0
	uint16_t dataflash;	/* data flash size in bytes 0=none                     */
	uint8_t idlatches;	/* USERID MULTI-WORD LATCHES                           */
#endif
};

/******************************************************************************
 * MEMORY
 *****************************************************************************/

#define PIC14_WORD(X) (((X) * 8192.0) / 14) /* KB to words */

/*
 * MEMORY MAP
 */
#if 0
#define PIC14_CODE_LOW     (0x0000)	/* CODE ORIGIN  */
#endif
#define PIC14_CONFIG_OFFSET (7)		/* 2007 or 8007 */

/*
 * DEVICE ID / REVISION
 */
#define PIC14_DEVICEID_MASK   (0x3FE0)
#define PIC14_DEVICEREV_MASK  (0x001F)
#define PIC14_DEVICEID_MASK3  (0x3000)	/* PIC16F1788 PIC16F1455 */
#define PIC14_DEVICEREV_MASK3 (0x0FFF)	/* PIC16F1788 PIC16F1455 */

/*
 * PROGRAM / ERASE
 */
#define PIC14_TPROG_DEFAULT	(10000)	/* 10ms                                  */
#define PIC14_TERASE_DEFAULT	(10000)	/* 10ms DS30072B-page 2 PIC16F84A        */
#define PIC14_TPROG_20MS	(20000)	/* 20ms DS30072B-page 2 PIC16F84  EEPROM */
#define PIC14_TERASE_20MS	(20000)	/* 20ms DS30072B-page 2 PIC16F84  EEPROM */
#define PIC14_TERASE_30MS	(30000)	/* 30ms                 PIC16F73         */
#define PIC14_TDISCHARGE_DEFAULT (100)	/* 100us                                 */
#define PIC14_TDISCHARGE_300US	 (300)	/* 300us                                 */

/******************************************************************************
 * PICMicro devices
 *****************************************************************************/

/*
 * DS30262E (MATURE PRODUCT 2013-11-12)
 *  PIC16CR83 No embedded device id (OTP)
 *  PIC16CR84 No embedded device id (OTP)
 *  PIC16F83  No embedded device id
 *  PIC16F84  No embedded device id
 *  PIC16F84A --00 0101 011X XXXX
 */
#define DS30262E (30262)
#if 0
#define PIC16CR83 (0x0560 + 0x01)
#define PIC16CR84 (0x0560 + 0x02)
#endif
#define PIC16F83  (0x0560 + 0x03)
#define PIC16F84  (0x0560 + 0x04)
#define PIC16F84A (0x0560)

/*
 * DS30189D
 *  PIC16C84 No embedded device id (EEPROM)
 */
#define DS30189D (30189)
#define PIC16C84 (0x0560 + 0x05)

/*
 * DS30034D (DS30277D PAGE 3-161) (MATURE PRODUCT 2013-11-12)
 *  PIC16F627 --00 0111 101X XXXX
 *  PIC16F628 --00 0111 110X XXXX
 */
#define DS30034D (30034)
#define PIC16F627 (0x07a0)
#define PIC16F628 (0x07c0)

/*
 * DS39025F (MATURE PRODUCT 2013-11-12)
 *  PIC16F870 --00 1101 000X XXXX
 *  PIC16F871 --00 1101 001X XXXX
 *  PIC16F872 --00 1000 111X XXXX
 *  PIC16F873 --00 1001 011X XXXX
 *  PIC16F874 --00 1001 001X XXXX
 *  PIC16F876 --00 1001 111X XXXX
 *  PIC16F877 --00 1001 101X XXXX
 */
#define DS39025F (39025)
#define PIC16F870 (0x0d00)
#define PIC16F871 (0x0d20)
#define PIC16F872 (0x08e0)
#define PIC16F873 (0x0960)
#define PIC16F874 (0x0920)
#define PIC16F876 (0x09e0)
#define PIC16F877 (0x09a0)

/*
 * DS39589C (MATURE PRODUCT 2013-11-12)
 *  PIC16F873A --00 1110 0100 XXXX
 *  PIC16F874A --00 1110 0110 XXXX
 *  PIC16F876A --00 1110 0000 XXXX
 *  PIC16F877A --00 1110 0010 XXXX
 */
#define DS39589C (39589)
#define PIC16F873A (0x0e40)
#define PIC16F874A (0x0e60)
#define PIC16F876A (0x0e00)
#define PIC16F877A (0x0e20)

/*
 * DS39607B
 *  PIC16F87 --00 0111 0010 XXXX
 *  PIC16F88 --00 0111 0110 XXXX
 */
#define DS39607B (39607)
#define PIC16F87 (0x0720)
#define PIC16F88 (0x0760)

/*
 * DS41191C
 *  PIC12F629 --00 1111 100X XXXX
 *  PIC12F675 --00 1111 110X XXXX
 *  PIC16F630 --01 0000 110X XXXX
 *  PIC16F676 --01 0000 111X XXXX
 */
#define DS41191C (41191)
#define PIC12F629 (0x0f80)
#define PIC12F675 (0x0fc0)
#define PIC16F630 (0x10c0)
#define PIC16F676 (0x10e0)

#define PIC14_OSCCALADDR  (0x03ff) /* DS41191C ONLY */
#define PIC14_BANDGAPADDR (0x2007) /* DS41191C ONLY */
#define PIC14_BANDGAPMASK (0x3000) /* DS41191C ONLY */
#define PIC14_CONFIGMASK  (0x01ff) /* DS41191C ONLY */

/*
 * DS41196F
 *  PIC16F627A --01 0000 010X XXXX
 *  PIC16F628A --01 0000 011X XXXX
 *  PIC16F648A --01 0001 000X XXXX
 */
#define DS41196F (41196)
#define PIC16F627A (0x1040)
#define PIC16F628A (0x1060)
#define PIC16F648A (0x1100)

/*
 * DS39603C
 *  PIC16F818 --00 0100 1100 XXXX
 *  PIC16F819 --00 0100 1110 XXXX
 */
#define DS39603C (39603)
#define PIC16F818 (0x04c0)
#define PIC16F819 (0x04e0)

/*
 * DS40245B
 *  PIC16F716 --01 0001 010X XXXX
 */
#define DS40245B (40245)
#define PIC16F716 (0x1140)

/*
 * DS41204G
 *  PIC12F635 --00 1111 101X XXXX
 *  PIC12F683 --00 0100 011X XXXX
 *  PIC16F631 --01 0100 001X XXXX
 *  PIC16F636 --01 0000 101X XXXX The 16F636 and 16F639 share the same device ID
 *  PIC16F639 --01 0000 101X XXXX The 16F636 and 16F639 share the same device ID
 *  PIC16F677 --01 0100 010X XXXX
 *  PIC16F684 --01 0000 100X XXXX
 *  PIC16F685 --00 0100 101X XXXX
 *  PIC16F687 --01 0011 001X XXXX
 *  PIC16F688 --01 0001 100X XXXX
 *  PIC16F689 --01 0011 010X XXXX
 *  PIC16F690 --01 0100 000X XXXX
 */
#define DS41204G (41204)
#define PIC12F635 (0x0fa0)
#define PIC12F683 (0x0460)
#define PIC16F631 (0x1420)
#define PIC16F636 (0x10a0)
#define PIC16F639 (0x10a0)
#define PIC16F677 (0x1440)
#define PIC16F684 (0x1080)
#define PIC16F685 (0x04a0)
#define PIC16F687 (0x1320)
#define PIC16F688 (0x1180)
#define PIC16F689 (0x1340)
#define PIC16F690 (0x1400)

/*
 * PIC16F72X series devices.
 *
 * These devices say that they require a VPP voltage of 9V D.C. and not 13V D.C
 * as supplied by the Velleman K8048, however, in tests 9V fails to work on the
 * K8048 and in fact the default (current limited) voltage of 13V works without
 * a problem.
 *
 * The L variant devices demand a lower VDD which is not possible on the Velleman
 * K8048 and are therefore incompatible.
 *
 * DS41332D
 *  PIC16F722   --01 1000 100X XXXX
 *  PIC16F722A  --01 1011 001X XXXX
 *  PIC16F723   --01 1000 011X XXXX
 *  PIC16F723A  --01 1011 000X XXXX
 *  PIC16F724   --01 1000 010X XXXX
 *  PIC16F726   --01 1000 001X XXXX
 *  PIC16F727   --01 1000 000X XXXX
 *  PIC16LF722  --01 1001 100X XXXX
 *  PIC16LF722A --01 1011 011X XXXX
 *  PIC16LF723  --01 1001 011X XXXX
 *  PIC16LF723A --01 1011 010X XXXX
 *  PIC16LF724  --01 1001 010X XXXX
 *  PIC16LF726  --01 1001 001X XXXX
 *  PIC16LF727  --01 1001 000X XXXX
 */
#define DS41332D (41332)
#define PIC16F722   (0x1880)
#define PIC16F722A  (0x1B20)
#define PIC16F723   (0x1860)
#define PIC16F723A  (0x1B00)
#define PIC16F724   (0x1840)
#define PIC16F726   (0x1820)
#define PIC16F727   (0x1800)
#define PIC16LF722  (0x1980)
#define PIC16LF722A (0x1B60)
#define PIC16LF723  (0x1960)
#define PIC16LF723A (0x1B40)
#define PIC16LF724  (0x1940)
#define PIC16LF726  (0x1920)
#define PIC16LF727  (0x1900)

/*
 * DS41409B
 *  PIC16F720   --01 1100 000X XXXX
 *  PIC16F720   --01 1100 001X XXXX
 *  PIC16LF720  --01 1100 010X XXXX
 *  PIC16LF721  --01 1100 011X XXXX
 */
#define DS41409B (41409)
#define PIC16F720   (0x1c00)
#define PIC16F721   (0x1c20)
#define PIC16LF720  (0x1c40)
#define PIC16LF721  (0x1c60)

/*
 * DS41405A
 *  PIC16F707   --01 1010 110X XXXX
 *  PIC16LF707  --01 1010 111X XXXX
 */
#define DS41405A (41405)
#define PIC16F707  (0x1AC0)
#define PIC16LF707 (0x1AE0)

/*
 * DS41284E
 *  PIC12F615	--10 0001 100X XXXX
 *  PIC12HV615	--10 0001 101X XXXX
 *  PIC12F617	--01 0011 011X XXXX
 *  PIC16F616	--01 0010 010X XXXX
 *  PIC16HV616	--01 0010 011X XXXX
 *  PIC12F609	--10 0010 010X XXXX
 *  PIC12HV609	--10 0010 100X XXXX
 *  PIC16F610	--10 0010 011X XXXX
 *  PIC16HV610	--10 0010 101X XXXX
 */
#define DS41284E (41284)
#define PIC12F615  (0x2180)
#define PIC12HV615 (0x21a0)
#define PIC12F617  (0x1360)
#define PIC16F616  (0x1240)
#define PIC16HV616 (0x1260)
#define PIC12F609  (0x2240)
#define PIC12HV609 (0x2280)
#define PIC16F610  (0x2260)
#define PIC16HV610 (0x22a0)

/*
 * DS41287D
 *  PIC16F882 --10 0000 000X XXXX
 *  PIC16F883 --10 0000 001X XXXX
 *  PIC16F884 --10 0000 010X XXXX
 *  PIC16F886 --10 0000 011X XXXX
 *  PIC16F887 --10 0000 100X XXXX
 */
#define DS41287D (41287)
#define PIC16F882 (0x2000)
#define PIC16F883 (0x2020)
#define PIC16F884 (0x2040)
#define PIC16F886 (0x2060)
#define PIC16F887 (0x2080)

/*
 * DS41572D
 *  PIC10F320  --10 1001 101X XXXX
 *  PIC10F322  --10 1001 100X XXXX
 *  PIC10LF320 --10 1001 111X XXXX
 *  PIC10LF322 --10 1001 110X XXXX
 */
#define DS41572D (41572)
#define PIC10F320  (0x29A0)
#define PIC10F322  (0x2980)
#define PIC10LF320 (0x29E0)
#define PIC10LF322 (0x29C0)

/*
 * DS41573C
 *  PIC12F1501  --10 1100 110X XXXX
 *  PIC16F1503  --10 1100 111X XXXX
 *  PIC16F1507  --10 1101 000X XXXX
 *  PIC16F1508  --10 1101 001X XXXX
 *  PIC16F1509  --10 1101 010X XXXX
 *  PIC12LF1501 --10 1100 100X XXXX
 *  PIC16LF1503 --10 1101 101X XXXX
 *  PIC16LF1507 --10 1101 110X XXXX
 *  PIC16LF1508 --10 1101 111X XXXX
 *  PIC16LF1509 --10 1110 000X XXXX
 */
#define DS41573C (41573)
#define PIC12F1501  (0x2CC0)
#define PIC16F1503  (0x2CE0)
#define PIC16F1507  (0x2D00)
#define PIC16F1508  (0x2D20)
#define PIC16F1509  (0x2D40)
#define PIC12LF1501 (0x2C80)
#define PIC16LF1503 (0x2DA0)
#define PIC16LF1507 (0x2DC0)
#define PIC16LF1508 (0x2DE0)
#define PIC16LF1509 (0x2E00)

/*
 * DS30324B
 *  PIC16F73 --00 0110 000X XXXX
 *  PIC16F74 --00 0110 001X XXXX
 *  PIC16F76 --00 0110 010X XXXX
 *  PIC16F77 --00 0110 011X XXXX
 */
#define DS30324B (30324)
#define PIC16F73 (0x0600)
#define PIC16F74 (0x0620)
#define PIC16F76 (0x0640)
#define PIC16F77 (0x0660)

/*
 * DS41439A
 *  PIC12F1840  --01 1011 100X XXXX
 *  PIC16F1847  --01 0100 100X XXXX
 *  PIC12LF1840 --01 1011 110X XXXX
 *  PIC16LF1847 --01 0100 101X XXXX
 */
#define DS41439A (41439)
#define PIC12F1840  (0x1D80)
#define PIC16F1847  (0x1480)
#define PIC12LF1840 (0x1DC0)
#define PIC16LF1847 (0x14A0)

/*
 * DS41397B
 *  PIC16F1933  --10 0011 000X XXXX
 *  PIC16F1934  --10 0011 010X XXXX
 *  PIC16F1936  --10 0011 011X XXXX
 *  PIC16F1937  --10 0011 100X XXXX
 *  PIC16F1938  --10 0011 101X XXXX
 *  PIC16F1939  --10 0011 110X XXXX
 *  PIC16F1946  --10 0101 000X XXXX
 *  PIC16F1947  --10 0101 001X XXXX
 *  PIC16LF1933 --10 0100 000X XXXX
 *  PIC16LF1934 --10 0100 010X XXXX
 *  PIC16LF1936 --10 0100 011X XXXX
 *  PIC16LF1937 --10 0100 100X XXXX
 *  PIC16LF1938 --10 0100 101X XXXX
 *  PIC16LF1939 --10 0100 110X XXXX
 *  PIC16LF1946 --10 0101 100X XXXX
 *  PIC16LF1947 --10 0101 101X XXXX
 *  PIC16LF1902 --10 1100 001X XXXX
 *  PIC16LF1903 --10 1100 000X XXXX
 *  PIC16LF1904 --10 1100 100X XXXX
 *  PIC16LF1906 --10 1100 011X XXXX
 *  PIC16LF1907 --10 1100 010X XXXX
 */
#define DS41397B (41397)
#define PIC16F1933  (0x2300)
#define PIC16F1934  (0x2340)
#define PIC16F1936  (0x2360)
#define PIC16F1937  (0x2380)
#define PIC16F1938  (0x23A0)
#define PIC16F1939  (0x23C0)
#define PIC16F1946  (0x2500)
#define PIC16F1947  (0x2520)
#define PIC16LF1933 (0x2400)
#define PIC16LF1934 (0x2440)
#define PIC16LF1936 (0x2460)
#define PIC16LF1937 (0x2480)
#define PIC16LF1938 (0x24A0)
#define PIC16LF1939 (0x24C0)
#define PIC16LF1946 (0x2580)
#define PIC16LF1947 (0x25A0)
#define PIC16LF1902 (0x2C20)
#define PIC16LF1903 (0x2C00)
#define PIC16LF1904 (0x2C80)
#define PIC16LF1906 (0x2C60)
#define PIC16LF1907 (0x2C40)

/*
 * DS41244F
 *  PIC16F917 --01 0011 1000 XXXX
 *  PIC16F916 --01 0011 1010 XXXX
 *  PIC16F914 --01 0011 1100 XXXX
 *  PIC16F913 --01 0011 1110 XXXX
 *  PIC16F946 --01 0100 0110 XXXX
 */
#define DS41244F (41244)
#define PIC16F917 (0x1380)
#define PIC16F916 (0x13a0)
#define PIC16F914 (0x13c0)
#define PIC16F913 (0x13e0)
#define PIC16F946 (0x1460)

/*
 * DS41390D
 *  PIC16F1826	--10 0111 100X XXXX
 *  PIC16F1827	--10 0111 101X XXXX
 *  PIC16LF1826	--10 1000 100X XXXX
 *  PIC16LF1827	--10 1000 101X XXXX
 *  PIC16F1823	--10 0111 001X XXXX
 *  PIC16LF1823	--10 1000 001X XXXX
 *  PIC12F1822	--10 0111 000X XXXX
 *  PIC12LF1822	--10 1000 000X XXXX
 *  PIC16F1824	--10 0111 010X XXXX
 *  PIC16LF1824	--10 1000 010X XXXX
 *  PIC16F1825	--10 0111 011X XXXX
 *  PIC16LF1825	--10 1000 011X XXXX
 *  PIC16F1828	--10 0111 110X XXXX
 *  PIC16LF1828	--10 1000 110X XXXX
 *  PIC16F1829	--10 0111 111X XXXX
 *  PIC16LF1829	--10 1000 111X XXXX
 */
#define DS41390D (41390)
#define PIC16F1826  (0x2780)
#define PIC16F1827  (0x27a0)
#define PIC16LF1826 (0x2880)
#define PIC16LF1827 (0x28a0)
#define PIC16F1823  (0x2720)
#define PIC16LF1823 (0x2820)
#define PIC12F1822  (0x2700)
#define PIC12LF1822 (0x2800)
#define PIC16F1824  (0x2740)
#define PIC16LF1824 (0x2840)
#define PIC16F1825  (0x2760)
#define PIC16LF1825 (0x2860)
#define PIC16F1828  (0x27c0)
#define PIC16LF1828 (0x28c0)
#define PIC16F1829  (0x27e0)
#define PIC16LF1829 (0x28e0)

/*
 * DS30492B
 *  PIC16F737 --00 1011 101X XXXX
 *  PIC16F747 --00 1011 111X XXXX
 *  PIC16F767 --00 1110 101X XXXX
 *  PIC16F777 --00 1101 111X XXXX
 */
#define DS30492B (30492)
#define PIC16F737 (0x0BA0)
#define PIC16F747 (0x0BE0)
#define PIC16F767 (0x0EA0)
#define PIC16F777 (0x0DE0)

/*
 * DS41457E
 *  DEVID [8006]             REVID-
 *  PIC16F1782  --10 1010 000X XXXX
 *  PIC16LF1782 --10 1010 101X XXXX
 *  PIC16F1783  --10 1010 001X XXXX
 *  PIC16LF1783 --10 1010 110X XXXX
 *  PIC16F1784  --10 1010 010X XXXX
 *  PIC16LF1784 --10 1010 111X XXXX
 *  PIC16F1786  --10 1010 011X XXXX
 *  PIC16LF1786 --10 1011 000X XXXX
 *  PIC16F1787  --10 1010 100X XXXX
 *  PIC16LF1787 --10 1011 001X XXXX
 *
 *  REVID [8005] --10 XXXX XXXX XXXX (2XXX)
 *  DEVID [8006]
 *  PIC16F1788   --11 0000 0010 1011 (302B)
 *  PIC16LF1788  --11 0000 0010 1101 (302D)
 *  PIC16F1789   --11 0000 0010 1010 (302A)
 *  PIC16LF1789  --11 0000 0010 1100 (302C)
 */
#define DS41457E (41457)
#define PIC16F1782  (0x2a00)
#define PIC16LF1782 (0x2aa0)
#define PIC16F1783  (0x2a20)
#define PIC16LF1783 (0x2ac0)
#define PIC16F1784  (0x2a40)
#define PIC16LF1784 (0x2ae0)
#define PIC16F1786  (0x2a60)
#define PIC16LF1786 (0x2b00)
#define PIC16F1787  (0x2a80)
#define PIC16LF1787 (0x2b20)
/* */
#define PIC16F1788  (0x302b) /* IF [8006] & 0x3000 == 0x3000 */
#define PIC16LF1788 (0x302d) /*    REVID = [8005] & 0x0FFF   */
#define PIC16F1789  (0x302a) /*    DEVID = [8006]            */
#define PIC16LF1789 (0x302c) /* ENDIF                        */

/*
 * DS41620C
 *  PIC16F1454  --11 0000 0010 0000
 *  PIC16LF1454 --11 0000 0010 0100
 *  PIC16F1455  --11 0000 0010 0001
 *  PIC16LF1455 --11 0000 0010 0101
 *  PIC16F1459  --11 0000 0010 0011
 *  PIC16LF1459 --11 0000 0010 0111
 */
#define DS41620C (41620)
#define PIC16F1454  (0x3020) /* IF [8006] & 0x3000 == 0x3000 */
#define PIC16LF1454 (0x3024) /*    REVID = [8005] & 0x0FFF   */
#define PIC16F1455  (0x3021) /*    DEVID = [8006]            */
#define PIC16LF1455 (0x3025) /*                              */
#define PIC16F1459  (0x3023) /*                              */
#define PIC16LF1459 (0x3027) /* ENDIF                        */

/*
 * DS41442B
 *  PIC16F1527   --01 0101 101X XXXX
 *  PIC16F1526   --01 0101 100X XXXX
 *  PIC16LF1527  --01 0101 111X XXXX
 *  PIC16LF1526  --01 0101 110X XXXX
 *  PIC16F1519   --01 0110 111X XXXX
 *  PIC16F1518   --01 0110 110X XXXX
 *  PIC16F1517   --01 0110 101X XXXX
 *  PIC16F1516   --01 0110 100X XXXX
 *  PIC16F1513   --01 0110 010X XXXX
 *  PIC16F1512   --01 0111 000X XXXX
 *  PIC16LF1519  --01 0111 111X XXXX
 *  PIC16LF1518  --01 0111 110X XXXX
 *  PIC16LF1517  --01 0111 101X XXXX
 *  PIC16LF1516  --01 0111 100X XXXX
 *  PIC16LF1513  --01 0111 010X XXXX
 *  PIC16LF1512  --01 0111 001X XXXX
 */
#define DS41442B (41442)
#define PIC16F1527  (0x15a0)
#define PIC16F1526  (0x1580)
#define PIC16LF1527 (0x15e0)
#define PIC16LF1526 (0x15c0)
#define PIC16F1519  (0x16e0)
#define PIC16F1518  (0x16c0)
#define PIC16F1517  (0x16a0)
#define PIC16F1516  (0x1680)
#define PIC16F1513  (0x1640)
#define PIC16F1512  (0x1700)
#define PIC16LF1519 (0x17e0)
#define PIC16LF1518 (0x17c0)
#define PIC16LF1517 (0x17a0)
#define PIC16LF1516 (0x1780)
#define PIC16LF1513 (0x1740)
#define PIC16LF1512 (0x1720)

/*
 * DS41561C
 *  PIC12F752  --01 0101 000X XXXX
 *  PIC12HV752 --01 0101 001X XXXX
 */
#define DS41561C (41561)
#define PIC12F752  (0x1500)
#define PIC12HV752 (0x1520)

/*
 * DS41642A
 *  PIC12LF1552 --10 1011 110X XXXX
 */
#define DS41642A (41642)
#define PIC12LF1552 (0x2BC0)

/*
 * DS40001743A
 *  PIC16LF1554 --10 1111 000X XXXX
 *  PIC16LF1559 --10 1111 001X XXXX
 */
#define DS40001743A (40001743)
#define PIC16LF1554 (0x2F00)
#define PIC16LF1559 (0x2F20)

/*
 * DS41686A
 *  REVID [2005]
 *  DEVID [2006]
 *  PIC16F753    0x3030
 *  PIC16HV753   0x3031
 */
#define DS41686A (41686)
#define PIC16F753  (0x3030)
#define PIC16HV753 (0x3031)

/*
 * DS41237D
 *  PIC16F785     --01 0010 000X XXXX
 *  PIC16HV785    --01 0010 001X XXXX
 *  PIC16F785-ICD ?
 */
#define DS41237D (41237)
#define PIC16F785  (0x1200)
#define PIC16HV785 (0x1220)
 
/*
 * DS40001683B
 *  REVID [8005]
 *  DEVID [8006]
 *  PIC16F1703	0x3061 0x2XXX
 *  PIC16LF1703	0x3063 0x2XXX
 *  PIC16F1704	0x3043 0x2XXX
 *  PIC16LF1704	0x3045 0x2XXX
 *  PIC16F1705	0x3055 0x2XXX
 *  PIC16LF1705	0x3057 0x2XXX
 *  PIC16F1707	0x3060 0x2XXX
 *  PIC16LF1707	0x3062 0x2XXX
 *  PIC16F1708	0x3042 0x2XXX
 *  PIC16LF1708	0x3044 0x2XXX
 *  PIC16F1709	0x3054 0x2XXX
 *  PIC16LF1709	0x3056 0x2XXX
 */
#define DS40001683B (40001683)
#define PIC16F1703  (0x3061)
#define PIC16LF1703 (0x3063)
#define PIC16F1704  (0x3043)
#define PIC16LF1704 (0x3045)
#define PIC16F1705  (0x3055)
#define PIC16LF1705 (0x3057)
#define PIC16F1707  (0x3060)
#define PIC16LF1707 (0x3062)
#define PIC16F1708  (0x3042)
#define PIC16LF1708 (0x3044)
#define PIC16F1709  (0x3054)
#define PIC16LF1709 (0x3056)

/*
 * DS40001720A
 *  PIC12F1612	--11 0000 0101 1000 0x2XXX
 *  PIC12LF1612 --11 0000 0101 1001 0x2XXX
 *  PIC16F1613  --11 0000 0100 1100 0x2XXX
 *  PIC16LF1613 --11 0000 0100 1101 0x2XXX
 */
#define DS40001720A (40001720)
#define PIC12F1612  (0x3058)
#define PIC12LF1612 (0x3059)
#define PIC16F1613  (0x304C)
#define PIC16LF1613 (0x304D)

/*
 * DS40001720B
 *  PIC16F1614	--11 0000 0111 1000 0x2XXX
 *  PIC16LF1614	--11 0000 0111 1010 0x2XXX
 *  PIC16F1615	--11 0000 0111 1100 0x2XXX
 *  PIC16LF1615	--11 0000 0111 1110 0x2XXX
 *  PIC16F1618	--11 0000 0111 1001 0x2XXX
 *  PIC16LF1618	--11 0000 0111 1011 0x2XXX
 *  PIC16F1619	--11 0000 0111 1101 0x2XXX
 *  PIC16LF1619	--11 0000 0111 1111 0x2XXX
 */
#define DS40001720B (40001720)
#define PIC16F1614  (0x3078)
#define PIC16LF1614 (0x307A)
#define PIC16F1615  (0x307C)
#define PIC16LF1615 (0x307E)
#define PIC16F1618  (0x3079)
#define PIC16LF1618 (0x307B)
#define PIC16F1619  (0x307D)
#define PIC16LF1619 (0x307F)

/*
 * DS40001714C
 *  PIC16F1713  0x3049 0x2xxx
 *  PIC16LF1713 0x304B 0x2xxx
 *  PIC16F1716  0x3048 0x2xxx
 *  PIC16LF1716 0x304A 0x2xxx
 *  PIC16F1717  0x305C 0x2xxx
 *  PIC16LF1717 0x305F 0x2xxx
 *  PIC16F1718  0x305B 0x2xxx
 *  PIC16LF1718 0x305E 0x2xxx
 *  PIC16F1719  0x305A 0x2xxx
 *  PIC16LF1719 0x305D 0x2xxx
 */
#define DS40001714C (40001714)
#define PIC16F1713  (0x3049)
#define PIC16LF1713 (0x304B)
#define PIC16F1716  (0x3048)
#define PIC16LF1716 (0x304A)
#define PIC16F1717  (0x305C)
#define PIC16LF1717 (0x305F)
#define PIC16F1718  (0x305B)
#define PIC16LF1718 (0x305E)
#define PIC16F1719  (0x305A)
#define PIC16LF1719 (0x305D)

/*
 * DS40001713A
 *  PIC12F1571	0x3051 0x2xxx
 *  PIC12LF1571	0x3053 0x2xxx
 *  PIC12F1572	0x3050 0x2xxx
 *  PIC12LF1572	0x3052 0x2xxx
 */
#define DS40001713A (40001713)
#define PIC12F1571  (0x3051)
#define PIC12LF1571 (0x3053)
#define PIC12F1572  (0x3050)
#define PIC12LF1572 (0x3052)

/*
 * DS40001754A
 *  PIC16F1764  0x3080 0x2xxx
 *  PIC16LF1764 0x3082 0x2xxx
 *  PIC16F1765  0x3081 0x2xxx
 *  PIC16LF1765 0x3083 0x2xxx
 *  PIC16F1768  0x3084 0x2xxx
 *  PIC16LF1768 0x3086 0x2xxx
 *  PIC16F1769  0x3085 0x2xxx
 *  PIC16LF1769 0x3087 0x2xxx
 */
#define DS40001754A (40001754)
#define PIC16F1764  (0x3080)
#define PIC16LF1764 (0x3082)
#define PIC16F1765  (0x3081)
#define PIC16LF1765 (0x3083)
#define PIC16F1768  (0x3084)
#define PIC16LF1768 (0x3086)
#define PIC16F1769  (0x3085)
#define PIC16LF1769 (0x3087)

/*
 * DS40001766A
 *  PIC16F1574	0x3000 0x2xxx
 *  PIC16LF1574	0x3004 0x2xxx
 *  PIC16F1575	0x3001 0x2xxx
 *  PIC16LF1575	0x3005 0x2xxx
 *  PIC16F1578	0x3002 0x2xxx
 *  PIC16LF1578	0x3006 0x2xxx
 *  PIC16F1579	0x3003 0x2xxx
 *  PIC16LF1579	0x3007 0x2xxx
 */
#define DS40001766A (40001766)
#define PIC16F1574  (0x3000)
#define PIC16LF1574 (0x3004)
#define PIC16F1575  (0x3001)
#define PIC16LF1575 (0x3005)
#define PIC16F1578  (0x3002)
#define PIC16LF1578 (0x3006)
#define PIC16F1579  (0x3003)
#define PIC16LF1579 (0x3007)

/*
 * DS40001738A
 *  PIC16F18313  0x3066
 *  PIC16LF18313 0x3068
 *  PIC16F18323  0x3067
 *  PIC16LF18323 0x3069
 *
 * DS40001738B/C
 *  PIC16F18313  0x3034
 *  PIC16LF18313 0x3036
 *  PIC16F18323  0x3035
 *  PIC16LF18323 0x3037
 *
 * DS40001738A/B/C
 *  PIC16F18324  0x303A
 *  PIC16LF18324 0x303C
 *  PIC16F18344  0x303B
 *  PIC16LF18344 0x303D
 *  PIC16F18325  0x303E
 *  PIC16LF18325 0x3040
 *  PIC16F18345  0x303F
 *  PIC16LF18345 0x3041
 */
#define DS40001738A (40001738)
#define PIC16F18313  (0x3066)
#define PIC16F18323  (0x3067)
#define PIC16F18324  (0x303A)
#define PIC16F18344  (0x303B)
#define PIC16F18325  (0x303E)
#define PIC16F18345  (0x303F)
#define PIC16LF18313 (0x3068)
#define PIC16LF18323 (0x3069)
#define PIC16LF18324 (0x303C)
#define PIC16LF18344 (0x303D)
#define PIC16LF18325 (0x3040)
#define PIC16LF18345 (0x3041)

/*
 * DS40001738D
 */
#define DS40001738D (40001738)
#define PIC16F18326  (0x30A4)
#define PIC16F18346  (0x30A5)
#define PIC16LF18326 (0x30A6)
#define PIC16LF18346 (0x30A7)

/*
 * DS40001792A
 *  PIC16F1773	0x308A
 *  PIC16LF1773	0x308C
 *  PIC16F1776	0x308B
 *  PIC16LF1776	0x308D
 *  PIC16F1777	0x308E
 *  PIC16LF1777	0x3091
 *  PIC16F1778	0x308F
 *  PIC16LF1778	0x3092
 *  PIC16F1779	0x3090
 *  PIC16LF1779	0x3093
 */
#define DS40001792A (40001792)
#define PIC16F1773  (0x308A)
#define PIC16LF1773 (0x308C)
#define PIC16F1776  (0x308B)
#define PIC16LF1776 (0x308D)
#define PIC16F1777  (0x308E)
#define PIC16LF1777 (0x3091)
#define PIC16F1778  (0x308F)
#define PIC16LF1778 (0x3092)
#define PIC16F1779  (0x3090)
#define PIC16LF1779 (0x3093)

/*
 * DS40001796A
 *  PIC16LF1566 0x3046 0x2xxx
 *  PIC16LF1567 0x3047 0x2xxx
 */
#define DS40001796A (40001796)
#define PIC16LF1566 (0x3046)
#define PIC16LF1567 (0x3047)

/******************************************************************************/

uint32_t pic14_arch(void);
void pic14_selector(void);
void pic14_program_verify(void);
void pic14_standby(void);
uint16_t pic14_read_program_memory_increment(void);
uint8_t  pic14_read_data_memory_increment(void);
void pic14_bulk_erase(void);
void pic14_row_erase(uint32_t, uint32_t);
int pic14_read_config_memory(void);
uint32_t pic14_get_program_size(uint32_t *);
uint32_t pic14_get_data_size(uint32_t *);
uint32_t pic14_read_program_memory_block(uint32_t *, uint32_t, uint32_t);
uint32_t pic14_read_data_memory_block(uint16_t *, uint32_t, uint16_t);
uint32_t pic14_write_word(uint16_t);
uint16_t pic14_osccal_read(void);
uint32_t pic14_osccal_write(uint16_t);
uint32_t pic14_bandgap_write(uint16_t);
uint32_t pic14_write_osccal(uint16_t);
uint32_t pic14_write_bandgap(uint16_t);
uint32_t pic14_write_calib(uint16_t, uint16_t);
uint32_t pic14_write_config(void);
uint16_t pic14_getregion(uint16_t);
uint16_t pic14_initregion(uint16_t, uint16_t *);
void pic14_loadregion(uint16_t, uint16_t);
uint32_t pic14_program_data(uint32_t, pic_data *);
void pic14_program_end(int);
uint32_t pic14_verify_data(uint32_t, pic_data *, uint32_t *);
void pic14_view_data(pic_data *);
void pic14_dumpdeviceid(void);
void pic14_dumposccal(void);
void pic14_dumpconfig(int);
void pic14_dumpconfig_16f627(uint16_t);
void pic14_dumpconfig_12f675(uint16_t);
void pic14_dumpconfig_16f877a(uint16_t);
void pic14_dumpconfig_16f88(uint16_t, uint16_t);
void pic14_dumpconfig_16f628a(uint16_t);
void pic14_dumpconfig_12f683(uint16_t);
void pic14_dumpconfig_16f886(uint16_t, uint16_t);
void pic14_dumphexcode(uint32_t, uint32_t, uint32_t *);
void pic14_dumpinhxcode(uint32_t, uint32_t, uint32_t *);
void pic14_dumphexdata(uint32_t, uint32_t, uint16_t *);
void pic14_dumpinhxdata(uint32_t, uint32_t, uint16_t *);
void pic14_dumpdevice(void);

#endif /* !_PIC14_H */
