//
// Common USB routines for Linux and Mac OSX
//
// By Jeff Post, j_post <AT> pacbell <DOT> net
// To contact the author, please include the word "pk2cmd" in your subject line,
//  otherwise your email may be rejected as spam.
//
//                            Software License Agreement
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
//
#ifndef	WIN32

#include	<stdio.h>
#include <unistd.h>
#include	<ctype.h>
#include	<string.h>

#include	"stdafx.h"
#include	"pk2usb.h"

// Prototypes
//   none

// Data

#ifdef VERBOSE
bool	verbose = 1;		// generate user messages
#else
bool	verbose = 0;		// don't generate user messages
#endif

int	pickit2mode = 0;
int	pickit2firmware = 0;
int	targetPower = 0;

int	pickit_interface = 0;
int	usbdebug = USB_DEBUG_FLAGS;
FILE	*usbFile = NULL;

byte	cmd[reqLen + 1];

struct scriptInterpreter scriptInterpreter[] = {
	{0xff, 0, (char *) "VDD_ON"},
	{0xfe, 0, (char *) "VDD_OFF"},
	{0xfd, 0, (char *) "VDD_GND_ON"},
	{0xfc, 0, (char *) "VDD_GND_OFF"},
	{0xfb, 0, (char *) "VPP_ON"},
	{0xfa, 0, (char *) "VPP_OFF"},
	{0xf9, 0, (char *) "VPP_PWM_ON"},
	{0xf8, 0, (char *) "VPP_PWM_OFF"},
	{0xf7, 0, (char *) "MCLR_GND_ON"},
	{0xf6, 0, (char *) "MCLR_GND_OFF"},
	{0xf5, 0, (char *) "BUSY_LED_ON"},
	{0xf4, 0, (char *) "BUSY_LED_OFF"},
	{0xf3, 1, (char *) "SET_ICSP_PINS"},
	{0xf2, 1, (char *) "WRITE_BYTE_LITERAL"},
	{0xf1, 0, (char *) "WRITE_BYTE_BUFFER"},
	{0xf0, 0, (char *) "READ_BYTE_BUFFER"},
	{0xef, 0, (char *) "READ_BYTE"},
	{0xee, 2, (char *) "WRITE_BITS_LITERAL"},
	{0xed, 1, (char *) "WRITE_BITS_BUFFER"},
	{0xec, 1, (char *) "READ_BITS_BUFFER"},
	{0xeb, 1, (char *) "READ_BITS"},
	{0xea, 1, (char *) "SET_ICSP_SPEED"},
	{0xe9, 2, (char *) "LOOP"},
	{0xe8, 1, (char *) "DELAY_LONG"},
	{0xe7, 1, (char *) "DELAY_SHORT"},
	{0xe6, 2, (char *) "IF_EQ_GOTO"},
	{0xe5, 2, (char *) "IF_GT_GOTO"},
	{0xe4, 1, (char *) "GOTO_INDEX"},
	{0xe3, 0, (char *) "EXIT_SCRIPT"},
	{0xe2, 1, (char *) "PEEK_SFR"},
	{0xe1, 2, (char *) "POKE_SFR"},
	{0xe0, 0, (char *) "ICDSLAVE_RX"},
	{0xdf, 1, (char *) "ICDSLAVE_TX_LIT"},
	{0xde, 0, (char *) "ICDSLAVE_TX_BUF"},
	{0xdd, 1, (char *) "LOOPBUFFER"},
	{0xdc, 0, (char *) "ICSP_STATES_BUFFER"},
	{0xdb, 0, (char *) "POP_DOWNLOAD"},
	{0xda, 2, (char *) "COREINST18"},
	{0xd9, 3, (char *) "COREINST24"},
	{0xd8, 0, (char *) "NOP24"},
	{0xd7, 0, (char *) "VISI24"},
	{0xd6, 0, (char *) "RD2_BYTE_BUFFER"},
	{0xd5, 1, (char *) "RD2_BITS_BUFFER"},
	{0xd4, 1, (char *) "WRITE_BUFWORD_W"},
	{0xd3, 1, (char *) "WRITE_BUFBYTE_W"},
	{0xd2, 1, (char *) "CONST_WRITE_DL"},
	{0xaf, 0, (char *) "SCRIPT_BUFFER_CHKSM"},
	{0xae, 0, (char *) "RESET"},
	{0xad, 0, (char *) "END_OF_BUFFER"},
	{0xac, 0, (char *) "UPLOAD_DATA_NOLEN"},
	{0xab, 0, (char *) "CLR_SCRIPT_BUFFER"},
	{0xaa, 0, (char *) "UPLOAD_DATA"},
	{0xa9, 0, (char *) "CLR_UPLOAD_BUFFER"},
	{0xa8, 255, (char *) "DOWNLOAD_DATA"},
	{0xa7, 0, (char *) "CLR_DOWNLOAD_BUFFER"},
	{0xa6, 255, (char *) "EXECUTE_SCRIPT"},
	{0xa5, 2, (char *) "RUN_SCRIPT"},
	{0xa4, 254, (char *) "DOWNLOAD_SCRIPT"},
	{0xa3, 0, (char *) "READ_VOLTAGES"},
	{0xa2, 0, (char *) "READ_STATUS"},
	{0xa1, 3, (char *) "SETVPP"},
	{0xa0, 3, (char *) "SETVDD"},
	{0, 0, (char *) "end marker"},
};

// The following must match the script buffer reserved locations
// defined in PICkitFunctions.h.

#define	SCRIPT_MAX	29

char	*scriptNames[SCRIPT_MAX + 1] = {
	(char *) "SCR_PROG_ENTRY",
	(char *) "SCR_PROG_EXIT",
	(char *) "SCR_RD_DEVID",
	(char *) "SCR_PROGMEM_RD",
	(char *) "SCR_ERASE_CHIP_PREP",
	(char *) "SCR_PROGMEM_ADDRSET",
	(char *) "SCR_PROGMEM_WR_PREP",
	(char *) "SCR_PROGMEM_WR",
	(char *) "SCR_EE_RD_PREP",
	(char *) "SCR_EE_RD",
	(char *) "SCR_EE_WR_PREP",
	(char *) "SCR_EE_WR",
	(char *) "SCR_CONFIG_RD_PREP",
	(char *) "SCR_CONFIG_RD",
	(char *) "SCR_CONFIG_WR_PREP",
	(char *) "SCR_CONFIG_WR",
	(char *) "SCR_USERID_RD_PREP",
	(char *) "SCR_USERID_RD",
	(char *) "SCR_USERID_WR_PREP",
	(char *) "SCR_USERID_WR",
	(char *) "SCR_OSCCAL_RD",
	(char *) "SCR_OSCCAL_WR",
	(char *) "SCR_ERASE_CHIP",
	(char *) "SCR_ERASE_PROGMEM",
	(char *) "SCR_ERASE_EE",
	(char *)	"Invalid Script Command",
	(char *) "SCR_ROW_ERASE",
	(char *) "SCR_TESTMEM_RD",
	(char *) "SCR_EEROW_ERASE",
	(char *) "Unknown Script Command"
};

// Code

// Find and return index in script interpreter table.
// If invalid script code, return -1.

int indexScriptInterpreter(byte code)
{
	int	i;

	for (i=0; scriptInterpreter[i].scriptcode; i++)
	{
		if (code == scriptInterpreter[i].scriptcode)
			break;
	}

	if (!scriptInterpreter[i].scriptcode)
		i = -1;

	return i;
}

void showUSBCommand(byte *src, int len)
{
	int	i, j, k, size, index;
	byte	data, count;

	if (!usbFile)
		usbFile = stdout;

	if (pickit2mode == BOOTLOAD_MODE)
	{
		for (i=0; i<len; i++)
		{
			switch (src[i])
			{
				case GETVERSION:
					fprintf(usbFile, "Get Firmware Version\n");
					break;

				case CLR_DOWNLOAD_BFR:
					fprintf(usbFile, "Clear Download Buffer\n");
					break;

				case CLR_UPLOAD_BFR:
					fprintf(usbFile, "Clear Upload Buffer\n");
					break;

				case READFWFLASH:
					i++;
					size = (int) src[i++] & 0xff;				// byte count
					index = (int) src[i++] & 0xff;			// low adrs
					index += (((int) src[i] & 0xff) << 8);	// high adrs
					fprintf(usbFile,
						"Read firmware flash: %d bytes at address 0x%04x\n",
						size, index);
					i = len;
					break;

				case WRITEFWFLASH:
					i++;
					size = (int) src[i++] & 0xff;				// byte count
					index = (int) src[i++] & 0xff;			// low adrs
					index += (((int) src[i] & 0xff) << 8);	// high adrs
					fprintf(usbFile,
						"Write firmware flash: %d bytes at address 0x%04x\n",
						size, index);
					i = len;
					break;

				case ERASEFWFLASH:
					i++;
					size = (int) src[i++] & 0xff;				// 64 byte block count
					i++;
					size *= 64;
					index = (((int) src[i++] & 0xff) << 8);	// high adrs
					index += ((int) src[i] & 0xff);				// low adrs
					fprintf(usbFile, "Erase firmware flash: address 0x%04x-0x%04x\n",
						index, index + size - 1);
					break;

				case READFWEEDATA:
					fprintf(usbFile, "Read firmware EE data\n");
					break;

				case WRITEFWEEDATA:
					fprintf(usbFile, "Write firmware EE data\n");
					i = len;
					break;

				case NOOPERATION:
				case END_OF_BFR:			// end of buffer - no more data
					i = len;
					break;

				case RESETFWDEVICE:
					fprintf(usbFile, "Reset PICkit2 firmware\n");
					i = len;
					break;

				default:
					fprintf(usbFile, "UNKNOWN COMMAND: 0x%02x\n", src[i]);
					i = len;		// terminate if we got out of sync
					break;
			}
		}

		fflush(usbFile);
		return;
	}

	for (i=0; i<len; i++)
	{
		switch (src[i])
		{
			case ENTERBOOTLOADER:
				fprintf(usbFile, "Enter Boot Loader\n");
				break;

			case GETVERSION:
				fprintf(usbFile, "Get Firmware Version\n");
				break;

			case SETVDD:
				fprintf(usbFile, "Set Vdd\n");
				i += 3;
				break;

			case SETVPP:
				fprintf(usbFile, "Set Vpp\n");
				i += 3;
				break;

			case READ_STATUS:
				fprintf(usbFile, "Read Status\n");
				break;

			case READ_VOLTAGES:
				fprintf(usbFile, "Read Voltages\n");
				break;

			case DOWNLOAD_SCRIPT:
				size = src[i + 2];
				index = src[i + 1] & 0xff;

				if (index > SCRIPT_MAX)
					index = SCRIPT_MAX;

				fprintf(usbFile, "Download Script %s, %d bytes\n",
						scriptNames[index], size);
				i += (size + 2);
				break;

			case RUN_SCRIPT:
				size = src[i + 2];
				index = src[i + 1] & 0xff;

				if (index > SCRIPT_MAX)
					index = SCRIPT_MAX;

				if (!size)
					size = 256;

				fprintf(usbFile, "Run Script %s %d time",
						scriptNames[index], size);

				if (size == 1)
					fprintf(usbFile, "\n");
				else
					fprintf(usbFile, "s\n");

				i += 2;
				break;

			case EXECUTE_SCRIPT:
				i++;
				size = src[i++];
				fprintf(usbFile, "Execute Script %d bytes", size);

				if (usbdebug & USB_DEBUG_SCRIPT)
				{
					for (j=0; j<size; )
					{
						data = src[i + j];
						index = indexScriptInterpreter(data);
						fprintf(usbFile, "\n    ");

						if (index >= 0)
						{
							count = scriptInterpreter[index].args + 1;

							if (count < 250)
							{
								for (k=0; k<count; k++)
								{
									fprintf(usbFile, "%02x ", src[i + j]);
									j++;
								}

								fprintf(usbFile, " %s", scriptInterpreter[index].name);
							}
							else
							{
								fprintf(usbFile, "%02x %s",
									data, scriptInterpreter[index].name);
								j++;
							}
						}
						else
						{
							fprintf(usbFile, "%02x Invalid script code", data);
							j++;
						}
					}
				}

				fprintf(usbFile, "\n");
				i += (size - 1);
				break;

			case CLR_DOWNLOAD_BFR:
				fprintf(usbFile, "Clear Download Buffer\n");
				break;

			case DOWNLOAD_DATA:
				size = src[i + 1];
				fprintf(usbFile, "Download Data %d bytes ", size);
				i += (size + 1);
				break;

			case CLR_UPLOAD_BFR:
				fprintf(usbFile, "Clear Upload Buffer\n");
				break;

			case UPLOAD_DATA:
				fprintf(usbFile, "Upload Data\n");
				break;

			case CLR_SCRIPT_BFR:
				fprintf(usbFile, "Clear Script Buffer\n");
				break;

			case UPLOAD_DATA_NOLEN:
				fprintf(usbFile, "Upload Data No Length\n");
				break;

			case END_OF_BFR:			// end of buffer - no more data
				i = len;
				break;

			case RESET:
				fprintf(usbFile, "RESET PICkit2\n");
				break;

			case SCRIPT_BFR_CHKSM:
				fprintf(usbFile, "Script Buffer Checksum\n");
				break;

			case MCLR_GND_OFF:
				fprintf(usbFile, "MCLR Off\n");
				break;

			case MCLR_GND_ON:
				fprintf(usbFile, "MCLR On\n");
				break;

			case BUSY_LED_OFF:
				fprintf(usbFile, "Busy LED Off\n");
				break;

			case BUSY_LED_ON:
				fprintf(usbFile, "Busy LED On\n");
				break;

			case VPP_PWM_OFF:
				fprintf(usbFile, "Vpp pwm Off\n");
				break;

			case VPP_PWM_ON:
				fprintf(usbFile, "Vpp pwm On\n");
				break;

			case VPP_OFF:
				fprintf(usbFile, "Vpp Off\n");
				break;

			case VPP_ON:
				fprintf(usbFile, "Vpp On\n");
				break;

			case VDD_OFF:
				fprintf(usbFile, "Vdd Off\n");
				break;

			case VDD_ON:
				fprintf(usbFile, "Vdd On\n");
				break;

			case VDD_GND_OFF:
				fprintf(usbFile, "Vdd gnd Off\n");
				break;

			case VDD_GND_ON:
				fprintf(usbFile, "Vdd gnd On\n");
				break;

			case WR_INTERNAL_EE:
				fprintf(usbFile, "Write internal EEPROM: 0x%02x 0x%02x\n", src[i + 1], src[i + 2]);
				i += 2;
				break;

			case RD_INTERNAL_EE:
				fprintf(usbFile, "Read internal EEPROM: 0x%02x 0x%02x\n", src[i + 1], src[i + 2]);
				i += 2;
				break;

//			case :
//				fprintf(usbFile, "\n");
//				break;

			default:
				fprintf(usbFile, "UNKNOWN COMMAND: 0x%02x\n", src[i]);
				break;
		}
	}

	fflush(usbFile);
}

// Send a command to the PICkit2

void sendPickitCmd(pickit_dev *d, byte *src, int len)
{
	int	i;

	for (i=0; i<len; i++)
	{
		cmd[i] = *src;
		src++;
	}

	while (i < reqLen)
	{
		cmd[i] = 'Z';
		i++;
	}

	sendUSB(d, cmd, len);
}

// Send a script command to the PICkit2

void sendScriptCmd(pickit_dev *d, byte *src, int len)
{
	int	i;

	for (i=0; i<len; i++)
	{
		cmd[i] = *src;
		src++;
	}

	while (i < reqLen)
	{
		cmd[i] = END_OF_BFR;
		i++;
	}

	sendUSB(d, cmd, len);
}

// Power on the target

void enableTargetPower(pickit_dev *d)
{
	if (targetPower == SELFPOWER)	// ignore if target is self powered
		return;

	cmd[0] = CLR_UPLOAD_BFR;
	cmd[1] = EXECUTE_SCRIPT;			// turn power on
	cmd[2] = 2;
	cmd[3] = VDD_GND_OFF;
	cmd[4] = VDD_ON;
	sendScriptCmd(d, cmd, 5);
}

// Power off the target

void disableTargetPower(pickit_dev *d)
{
	if (targetPower == SELFPOWER)	// ignore if target is self powered
		return;

	cmd[0] = EXECUTE_SCRIPT;	// turn power off
	cmd[1] = 2;
	cmd[2] = VDD_OFF;
	cmd[3] = VDD_GND_ON;
	cmd[4] = EXECUTE_SCRIPT;
	cmd[5] = 2;
	cmd[6] = VPP_PWM_OFF;
	cmd[7] = VPP_OFF;
	sendScriptCmd(d, cmd, 8);
}

// Turn the device on

void pickitOn(pickit_dev *d)
{
	if (targetPower == SELFPOWER)	// ignore if target is self powered
		return;

	enableTargetPower(d);
	usleep(100000);
}

// Turn the device off

void pickitOff(pickit_dev *d)
{
	if (targetPower == SELFPOWER)	// ignore if target is self powered
		return;

	disableTargetPower(d);
}

CUsbhidioc::CUsbhidioc(void)
{
	m_UnitID[0] = 0; 
}

char *CUsbhidioc::GetPK2UnitID(void)
{
	return m_UnitID;
}

// Detect the PICkit2

bool CUsbhidioc::FindTheHID(int unitIndex)
{
	if (!deviceHandle)
		deviceHandle = usbPickitOpen(unitIndex, (char *) &m_UnitID);

	if (deviceHandle)
		return 1;

	return 0;
}

// Return 1 if read ok, else return 0
// int recvUSB(pickit_dev *d, int len, byte *dest) // returns 1 if ok

bool CUsbhidioc::ReadReport(char InBuffer[])
{
	int	i;

	if (!deviceHandle)
	{
			return 0;
	}

	i = recvUSB(deviceHandle, reqLen, (byte *) InBuffer);
	return i;
}

// Return 1 if write ok, else return 0
// int sendUSB(pickit_dev *d, byte *src, int len) - returns 1 if ok

bool CUsbhidioc::WriteReport(char OutputBuffer[], unsigned int nBytes)
{
	int	i;

	if (!deviceHandle)
	{
			return 0;
	}

	for (i = nBytes; i < 65; ++i)
        OutputBuffer [i] = 'Z';  // Pad buffer with Null Commands

	i = sendUSB(deviceHandle, (byte *) OutputBuffer, nBytes);
	return i;
}

// Close the channel

void CUsbhidioc::CloseReport(void)
{
	if (deviceHandle)
	{

		usb_release_interface(deviceHandle, pickit_interface);
		deviceHandle = NULL;
		pickit_interface = 0;
	}
}

// end

#endif	// not WIN32
