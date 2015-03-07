//                            Software License Agreement
//
// Copyright (c) 2005-2009, Microchip Technology Inc. All rights reserved.
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
// usbhidioc.cpp : USB HID I/O implementation file
/*
Date: 23 Nov 2004
Dan Butler, Microchip Technology
Version 1.0, Based on Jan Axelson's (jan@lvr.com) USBHIDIOC V2.0
Implements PC to device communication via the HID driver.

Written for Borland C++ Builder V6

Access is through three functions:
   WriteFile
   ReadFile
   CloseFile
Both Read and Write functions check to see if the device is opened on each call.
If not, it goes out and looks for the device and opens the file.  On failure,
the channel is closed so it will attempt to reopen the device on the next call.

bool WriteFile (char *buffer)
Assumes the contents of the buffer is correct and appropriate for the end
device.  It adds on the 0 to the beginning of the buffer and sends it to
the HID driver, which formats it and sends it to the device.

bool ReadFile (char *buffer)
Returns the buffer read from the device.


Project: usbhidioc.cpp
Version: 2.0
Date: 12/20/00
by Jan Axelson (jan@lvr.com)

Purpose: demonstrates USB communications with a HID-class device using overlapped I/O.

Description:
	Finds an attached HID-class device that matches specific vendor & product IDs.
	Retrieves the device's capabilities.

This application was created with Borland's C++ Builder 6's.

About overlapped I/O

Reading HID input reports is done with the API function ReadFile.
Non-overlapped ReadFile is a blocking call. If the device doesn't return the
expected amount of data, the application hangs and will wait forever.

With overlapped I/O, the call to ReadFile returns immediately. The application then calls
WaitForSingleObject, which returns when either the data has arrived or the specified timeout has elapsed.

*/

#include "stdafx.h"
#include "Windows.h"
#include "usbhidioc.h"

#include <wtypes.h>
#include <initguid.h>

extern "C" {
#include "hidsdi.h"
#include <setupapi.h>
}

#include <stdlib.h>
#include <malloc.h>

//function prototypes


/////////////////////////////////////////////////////////////////////////////
// CUsbhidioc




//Application global variables
//	HIDP_CAPS		Capabilities;   08Mar2005 - Capabilities call fails on WinXP
	DWORD			cbBytesRead;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
	bool			DeviceDetected = false;
	HANDLE			DeviceHandle;
	DWORD			dwError;
	HANDLE			hEventObject;
	HANDLE			hDevInfo;
	GUID			HidGuid;
	OVERLAPPED		HIDOverlapped;
	char			InputReport[65];
	ULONG			Length;
	DWORD			NumberOfBytesRead;
	HANDLE			ReadHandle;
	ULONG			Required;

CUsbhidioc  USB;

CUsbhidioc::CUsbhidioc(void)
   {
		m_UnitID[0] = 0; 
   }

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg message handlers

char* CUsbhidioc::GetPK2UnitID(void)
{
	return m_UnitID;
}

bool CUsbhidioc::FindTheHID(int unitNumber)
{
	//Use a series of API calls to find a HID with a matching Vendor and Product ID.

	HIDD_ATTRIBUTES			Attributes;
	SP_DEVICE_INTERFACE_DATA	   devInfoData;
	bool					LastDevice = FALSE;
	int						MemberIndex;
	LONG					Result;
	int						unitIndex;
	char					unitIDSerial[64];

	//These are the vendor and product IDs to look for.
	const unsigned int VendorID = 0x04d8;    // Uses Microchip's Vendor ID.
	const unsigned int ProductID = 0x0033;   // PICkit 2 Flash Starter Kit

	Length = 0;
	detailData = NULL;
	DeviceHandle=NULL;

	/*
	API function: HidD_GetHidGuid
	Get the GUID for all system HIDs.
	Returns: the GUID in HidGuid.
	*/

	HidD_GetHidGuid(&HidGuid);
	
	/*
	API function: SetupDiGetClassDevs
	Returns: a handle to a device information set for all installed devices.
	Requires: the GUID returned by GetHidGuid.
	*/
	
	hDevInfo=SetupDiGetClassDevs
		(&HidGuid,
		NULL, 
		NULL,
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
		
	devInfoData.cbSize = sizeof(devInfoData);

	//Step through the available devices looking for the one we want. 
	//Quit on detecting the desired device or checking all available devices without success.
	MemberIndex = 0;
	unitIndex = 0;
	LastDevice = FALSE;

	do
	{
		DeviceDetected=false;

		/*
		API function: SetupDiEnumDeviceInterfaces
		On return, MyDeviceInterfaceData contains the handle to a
		SP_DEVICE_INTERFACE_DATA structure for a detected device.
		Requires:
		The DeviceInfoSet returned in SetupDiGetClassDevs.
		The HidGuid returned in GetHidGuid.
		An index to specify a device.
		*/

		Result=SetupDiEnumDeviceInterfaces
			(hDevInfo,
			0,
			&HidGuid,
			MemberIndex,
			&devInfoData);

		if (Result != 0)
		{
			//A device has been detected, so get more information about it.

			/*
			API function: SetupDiGetDeviceInterfaceDetail
			Returns: an SP_DEVICE_INTERFACE_DETAIL_DATA structure
			containing information about a device.
			To retrieve the information, call this function twice.
			The first time returns the size of the structure in Length.
			The second time returns a pointer to the data in DeviceInfoSet.
			Requires:
			A DeviceInfoSet returned by SetupDiGetClassDevs
			The SP_DEVICE_INTERFACE_DATA structure returned by SetupDiEnumDeviceInterfaces.

			The final parameter is an optional pointer to an SP_DEV_INFO_DATA structure.
			This application doesn't retrieve or use the structure.
			If retrieving the structure, set
			MyDeviceInfoData.cbSize = length of MyDeviceInfoData.
			and pass the structure's address.
			*/

			//Get the Length value.
			//The call will return with a "buffer too small" error which can be ignored.
			Result = SetupDiGetDeviceInterfaceDetail
				(hDevInfo,
				&devInfoData,
				NULL,
				0,
				&Length,
				NULL);

			//Allocate memory for the hDevInfo structure, using the returned Length.
			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);

			//Set cbSize in the detailData structure.
			detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        // **** IMPORTANT ****
        // sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) should equal 5.
        // In C++ Builder, go to Project/Options/Advanced Compiler/Data Alignment
        // and select "byte" align.

			//Call the function again, this time passing it the returned buffer size.
			Result = SetupDiGetDeviceInterfaceDetail
				(hDevInfo,
				&devInfoData,
				detailData,
				Length,
				&Required,
				NULL);

                        Result = GetLastError ();
			//Open a handle to the device.

			/*
			API function: CreateFile
			Returns: a handle that enables reading and writing to the device.
			Requires:
			The DevicePath in the detailData structure
			returned by SetupDiGetDeviceInterfaceDetail.
			*/

			DeviceHandle=CreateFile
				(detailData->DevicePath,
				GENERIC_READ|GENERIC_WRITE,
				FILE_SHARE_READ|FILE_SHARE_WRITE,
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING,
				0,
				NULL);

			if ((int)DeviceHandle != -1)
			{
				/*
				API function: HidD_GetAttributes
				Requests information from the device.
				Requires: the handle returned by CreateFile.
				Returns: a HIDD_ATTRIBUTES structure containing
				the Vendor ID, Product ID, and Product Version Number.
				Use this information to decide if the detected device is
				the one we're looking for.
				*/

				//Set the Size to the number of bytes in the structure.
				Attributes.Size = sizeof(Attributes);

				Result = HidD_GetAttributes
					(DeviceHandle,
					&Attributes);

				if (Result != 0)
				{
					//Is it the desired device?
					DeviceDetected = false;

					if (Attributes.VendorID == VendorID)
					{
						if (Attributes.ProductID == ProductID)
						{
							//Both the Product and Vendor IDs match.
							if (unitNumber == unitIndex)
							{
								// Get Unit ID serial string
								HidD_GetSerialNumberString(DeviceHandle, unitIDSerial, 64);
								int x = 2;
								if ((unitIDSerial[0] == 0x09) || (unitIDSerial[0] == 0x00))
								{
									m_UnitID[0] = '-';
									m_UnitID[1] = 0;
								}
								else
								{
									m_UnitID[0] = unitIDSerial[0];
									for (; x < 28; x+=2)
									{
										m_UnitID[x/2] = unitIDSerial[x];
									    if ((unitIDSerial[x] == 0) || (unitIDSerial[x] == 0xE0))
										   break;
									    m_UnitID[x] = 0;
									    m_UnitID[x+1] = 0;
									}
								}
								DeviceDetected = true;
								PrepareForOverlappedTransfer();
							}
							else
							{
								CloseHandle(DeviceHandle);
								unitIndex++;
							}
						} //if (Attributes.ProductID == ProductID)

						else
							//The Product ID doesn't match.
							CloseHandle(DeviceHandle);
					} //if (Attributes.VendorID == VendorID)

					else
						//The Vendor ID doesn't match.
						CloseHandle(DeviceHandle);

				//Free the memory used by the detailData structure (no longer needed).
				free(detailData);
				}  //if (Result != 0)

				else
					//SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
					LastDevice=TRUE;
			}
		}

		//If we haven't found the device yet, and haven't tried every available device,
		//try the next one.
		MemberIndex = MemberIndex + 1;

	} //do
	while ((LastDevice == FALSE) && (DeviceDetected == false) && (MemberIndex < 24)); // check 24 devices max

	//Free the memory reserved for hDevInfo by SetupDiClassDevs.
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return DeviceDetected;
}



void CUsbhidioc::PrepareForOverlappedTransfer()
{
	//Get another handle to the device for the overlapped ReadFiles.
	ReadHandle=CreateFile
		(detailData->DevicePath,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);

	//Get an event object for the overlapped structure.

	/*API function: CreateEvent
	Requires:
	  Security attributes or Null
	  Manual reset (true). Use ResetEvent to set the event object's state to non-signaled.
	  Initial state (true = signaled)
	  Event object name (optional)
	Returns: a handle to the event object
	*/

	if (hEventObject == 0)
	{
		hEventObject = CreateEvent
			(NULL,
			TRUE,
			TRUE,
			(LPCSTR)"");

	//Set the members of the overlapped structure.
	HIDOverlapped.hEvent = hEventObject;
	HIDOverlapped.Offset = 0;
	HIDOverlapped.OffsetHigh = 0;
	}
}


bool CUsbhidioc::ReadReport(char InBuffer[])
{
	DWORD	Result;

	//Sleep(1);

	//Read a report from the device.

	/*API call:ReadFile
	'Returns: the report in InputReport.
	'Requires: a device handle returned by CreateFile
	'(for overlapped I/O, CreateFile must be called with FILE_FLAG_OVERLAPPED),
	'the Input report length in bytes returned by HidP_GetCaps,
	'and an overlapped structure whose hEvent member is set to an event object.
	*/


        //if (DeviceDetected == false)
        //    FindTheHID ();

        if (DeviceDetected)
        {
	    Result = ReadFile
		(ReadHandle,
		InputReport,
		65, //Capabilities.InputReportByteLength,     08Mar2005 - Capabilities call fails on WinXP
		&NumberOfBytesRead,
		(LPOVERLAPPED) &HIDOverlapped);


        	/*API call:WaitForSingleObject
	        'Used with overlapped ReadFile.
	        'Returns when ReadFile has received the requested amount of data or on timeout.
	        'Requires an event object created with CreateEvent
	        'and a timeout value in milliseconds.
	        */

	    Result = WaitForSingleObject
		(hEventObject,
		1000);

	    switch (Result)
	    {
	    case WAIT_OBJECT_0:
                {
		break;
                }
	    case WAIT_TIMEOUT:
                {
		//Cancel the Read operation.
		/*API call: CancelIo
		Cancels the ReadFile
                Requires the device handle.
                Returns non-zero on success.
		*/

		Result = CancelIo(ReadHandle);

		//A timeout may mean that the device has been removed.
		//Close the device handles and set DeviceDetected = False
		//so the next access attempt will search for the device.
		CloseHandle(ReadHandle);
		CloseHandle(DeviceHandle);
		DeviceDetected = FALSE;
		break;
                }
	    default:
                {
		break;
                }
	    }

	    // API call: ResetEvent
	    // Sets the event object to non-signaled.
	    // Requires a handle to the event object.
            // Returns non-zero on success.
	    ResetEvent(hEventObject);

            // copy array to destination buffer, stripping off the report ID.
            unsigned int i;
            for (i=0; i < 65 /*Capabilities.InputReportByteLength*/; ++i)
                InBuffer[i] = InputReport [i+1];
            Result = true;
        }
        else  // could not find the device
            Result = false;

        return Result;
}

bool CUsbhidioc::WriteReport(char OutputBuffer[], unsigned int nBytes)
{
    DWORD   BytesWritten = 0;
    ULONG   Result;
    char    OutputReport [65];
    unsigned int     i;
	//The first byte is the report number.
    OutputReport[0]=0;
    for (i=0; i < nBytes; i++)
        OutputReport [i+1] = OutputBuffer[i];
    for (i = nBytes+1; i < 65; ++i)
        OutputReport [i] = 'Z';  // Pad buffer with Null Commands

/*
	API Function: WriteFile
	Sends a report to the device.
	Returns: success or failure.
	Requires:
	The device handle returned by CreateFile.
	The Output Report length returned by HidP_GetCaps,
	A report to send.
	*/

    //if (DeviceDetected == false)
    //    FindTheHID ();

    if (DeviceDetected)
    {
        Result = WriteFile
		(DeviceHandle,
		OutputReport,
        65, //Capabilities.OutputReportByteLength,  08Mar2005 - Capabilities call fails on WinXP
		&BytesWritten,
		NULL);

        if (Result == 0)
        {
            Result = GetLastError ();
		//The WriteFile failed, so close the handle, display a message,
		//and set DeviceDetected to FALSE so the next attempt will look for the device.
		CloseHandle(DeviceHandle);
		CloseHandle(ReadHandle);
		DeviceDetected = false;
        }
    }
    else
        Result = false;
    return Result;
}


// Close the channel
void CUsbhidioc::CloseReport ()
{
    if (DeviceDetected == true)
    {
        CancelIo(ReadHandle);

    //A timeout may mean that the device has been removed.
    //Close the device handles and set DeviceDetected = False
    //so the next access attempt will search for the device.
        CloseHandle(ReadHandle);
        CloseHandle(DeviceHandle);
        DeviceDetected = false;
    }
}


