/* HID Report public interafaces

     Portions Copyright (c) 2005 Shigenobu Kimura, All rights reserved.

	 Other licenses included with this software do not apply to
	 the software code in this file.  The following license applies
	 to the contents of this file, and the contents of this file
	 only.

     Permission is hereby granted, free of charge, to any person
     obtaining a copy of this software and associated documentation
     files (the "Software"), to deal in the Software without restriction,
     including without limitation the rights to use, copy, modify,
     merge, publish, distribute, sublicense, and/or sell copies of
     the Software, and to permit persons to whom the Software is
     furnished to do so, subject to the following conditions:

     The above copyright notice and this permission notice shall be
     included in all copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
     BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
     AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
     OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
     IN THE SOFTWARE.

 */

#ifndef	WIN32

#ifndef HIDREPORT_H
#define HIDREPORT_H

#define REPORT_SIZE 64

/*
	hidreport_open :  Open HID device for reports.
	returns NULL if error.
*/

hidreport_t hidreport_open(int vendor_id, int product_id, int buffer_size, int unit);

void hidreport_close(hidreport_t hid);

/*
	hidreport_send : send report
	returns non-zero if error.
*/

int hidreport_send(hidreport_t rep, void *buf, int size);

/*
	hidreport_receive : receive report,
	sets *size to the number of bytes received.
	returns non-zero if error.
*/
int hidreport_receive(hidreport_t rep, void *buf, UInt32 *size);

/*
	hidreport_sendrec : send and receive report,
	returns non-zero if error.
*/
int hidreport_sendrec(hidreport_t rep, void *buf, int size);

/* hooks for pk2 */

int sendUSB(pickit_dev *d, unsigned char *src, int len);

int recvUSB(pickit_dev *d, int len, unsigned char *dest);

pickit_dev *usbPickitOpen(int unit, char *id);

void usb_release_interface(pickit_dev *deviceHandle, int pickit_interface);

#endif

#endif // WIN32
/* EOF */
