/**
 *
 * Windows implementation of serial port abstraction
 *
 * Copyright (C) Joseph Heenan 2005-2008, all rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Joseph Heenan nor the names of any other
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Joseph Heenan ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#include "serial.h"

/**
 * @param port COM port - eg "COM4:"
 * 
 * @param stopbits Not supported on windows - always 1 stop bit
 */
int serial_openport(const char *port, int baudrate, int stopbits)
{
    HANDLE       hPort;
    DCB          PortDCB;
    COMMTIMEOUTS CommTimeouts;
    char         portFname[128];

    strcpy(portFname, "\\\\.\\");
    strcat(portFname, port);
    
    // Open the serial port.
    hPort = CreateFile (portFname, // Pointer to the name of the port
                        GENERIC_READ | GENERIC_WRITE,
                                      // Access (read-write) mode
                        0,            // Share mode
                        NULL,         // Pointer to the security attribute
                        OPEN_EXISTING,// How to open the serial port
                        0,            // Port attributes
                        NULL);        // Handle to port with attribute
                                      // to copy

    if (hPort == INVALID_HANDLE_VALUE)
    {
        int dwError = GetLastError ();
        fprintf(stderr, "Unable to open serial port '%s' (%d)\n",
                port, dwError);
        return -1;
    }
    
    memset(&PortDCB, 0, sizeof(PortDCB));
    // Initialize the DCBlength member. 
    PortDCB.DCBlength = sizeof (DCB); 
    
    // Get the default port setting information.
    GetCommState (hPort, &PortDCB);
    
    // Change the DCB structure settings.
    PortDCB.BaudRate = baudrate;          // Current baud 
    PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
    PortDCB.fParity = FALSE;              // No parity checking 
    PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
    PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
    PortDCB.fDtrControl = DTR_CONTROL_ENABLE; 
                                          // DTR flow control type 
    PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
    PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
    PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
    PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
    PortDCB.fErrorChar = FALSE;           // Disable error replacement 
    PortDCB.fNull = FALSE;                // Disable null stripping 
    PortDCB.fRtsControl = RTS_CONTROL_ENABLE; 
                                          // RTS flow control 
    PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
                                          // error
    PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8 
    PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
    PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 

    // Configure the port according to the specifications of the DCB 
    // structure.
    if (!SetCommState (hPort, &PortDCB))
    {
        int dwError = GetLastError ();
        // Could not configure the serial port.
        fprintf(stderr, "Unable to configure the serial port (%d)\n", 
                dwError);
        return -1;
    }
    
    // Retrieve the time-out parameters for all read and write operations
    // on the port. 
    memset(&CommTimeouts, 0, sizeof(CommTimeouts));
    GetCommTimeouts (hPort, &CommTimeouts);
    
    // Change the COMMTIMEOUTS structure settings.
    CommTimeouts.ReadIntervalTimeout = MAXDWORD;  
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = MAXDWORD; /* no timeout */
    CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
    CommTimeouts.WriteTotalTimeoutConstant = 1000;    
    
    // Set the time-out parameters for all read and write operations
    // on the port. 
    if (!SetCommTimeouts (hPort, &CommTimeouts))
    {
        int dwError = GetLastError ();
        // Could not set the time-out parameters.
        fprintf(stderr,
                "Unable to set the time-out parameters (%d)\n", 
                dwError);
        return -1;
    }

    return (int)hPort;
}

int serial_write(int hPort, const char *ptr, int bytes)
{
    int send = -1;
    int ret;

    ret = WriteFile((HANDLE)hPort,        // Port handle
                    ptr,                  // Pointer to data to read
                    bytes,                // Number of bytes to read
                    &send,                // Pointer to number of bytes
                                          // written
                    NULL);                // Not overlapped
    if (ret == 0)
    {
        return -1;
    }
        
    return send;
}

int serial_read(int hPort, unsigned char *ptr, int bytes)
{
    int cnt = -1;
    int ret;
    HANDLE h = (HANDLE)hPort;

    ret = ReadFile(h,                   // Port handle
                   ptr,                  // Pointer to data to read
                   bytes,                // Number of bytes to read
                   &cnt,                 // Pointer to number of bytes
                                         // written
                   NULL);                // Not overlapped
    if (ret == 0)
    {   
        fprintf(stderr, "Failed to read from serial port\n");
        exit(1);
    }

    return cnt;
}

int serial_readtimeout(int fd, unsigned char *ptr, int bytes,
                       int timeout)
{
    int cnt = -1;
    int ret;
    HANDLE hPort = (HANDLE) fd;
    COMMTIMEOUTS CommTimeouts;
    COMMTIMEOUTS OrigCommTimeouts;

    // Retrieve the time-out parameters for all read and write operations
    // on the port. 
    memset(&CommTimeouts, 0, sizeof(CommTimeouts));
    GetCommTimeouts (hPort, &CommTimeouts);
    OrigCommTimeouts = CommTimeouts;
    
    // Change the COMMTIMEOUTS structure settings.
    CommTimeouts.ReadIntervalTimeout = MAXDWORD;  
    CommTimeouts.ReadTotalTimeoutMultiplier = 1;
    CommTimeouts.ReadTotalTimeoutConstant = timeout; /* 100 seconds timeout */
    CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
    CommTimeouts.WriteTotalTimeoutConstant = 1000;    
    
    // Set the time-out parameters for all read and write operations
    // on the port. 
    if (!SetCommTimeouts (hPort, &CommTimeouts))
    {
        int dwError = GetLastError ();
        // Could not set the time-out parameters.
        fprintf(stderr,
                "Unable to set the time-out parameters (%d)\n", 
                dwError);
        return -1;
    }

    ret = ReadFile(hPort,                // Port handle
                   ptr,                  // Pointer to data to read
                   bytes,                // Number of bytes to read
                   &cnt,                 // Pointer to number of bytes
                                         // written
                   NULL);                // Not overlapped
    if (ret == 0)
    {   
        int dwError = GetLastError ();
        fprintf(stderr, "Failed to read from serial port (%d)\n",
                dwError);
        exit(1);
    }
    if (cnt == 0)
    {
        /* timeout must have been hit */
        return -2;
    }

    /* restore previous timeouts */
    if (!SetCommTimeouts (hPort, &OrigCommTimeouts))
    {
        int dwError = GetLastError ();
        // Could not set the time-out parameters.
        fprintf(stderr,
                "Unable to set the time-out parameters (%d)\n", 
                dwError);
        return -1;
    }

    return cnt;
}


void serial_close(int fd)
{
    HANDLE h = (HANDLE)fd;

    CloseHandle(h);
}
