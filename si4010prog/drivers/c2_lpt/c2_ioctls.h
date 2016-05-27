/*
    This file implements the IOCTL codes for the Cygnal C2 driver
    Written by Wojciech M. Zabolotny (wzab@ise.pw.edu.pl)
    Published under the GPL (Gnu Public License) version 2.0 or later
*/
#define C2_IOC_MAGIC ('c')
#define C2_IOCRESET _IO(C2_IOC_MAGIC, 1)
#define C2_IOCAWRITE _IOW(C2_IOC_MAGIC,2,char)
#define C2_IOCDWRITE _IOC(_IOC_WRITE,C2_IOC_MAGIC,3,0)
#define C2_IOCD1WRITE _IOC(_IOC_WRITE,C2_IOC_MAGIC,3,1)
#define C2_IOCD2WRITE _IOC(_IOC_WRITE,C2_IOC_MAGIC,3,2)
#define C2_IOCD3WRITE _IOC(_IOC_WRITE,C2_IOC_MAGIC,3,3)
#define C2_IOCD4WRITE _IOC(_IOC_WRITE,C2_IOC_MAGIC,3,4)
#define C2_IOCAREAD _IOR(C2_IOC_MAGIC,4,char)
#define C2_IOCDREAD _IOC(_IOC_READ,C2_IOC_MAGIC,5,0)
#define C2_IOCD1READ _IOC(_IOC_READ,C2_IOC_MAGIC,5,1)
#define C2_IOCD2READ _IOC(_IOC_READ,C2_IOC_MAGIC,5,2)
#define C2_IOCD3READ _IOC(_IOC_READ,C2_IOC_MAGIC,5,3)
#define C2_IOCD4READ _IOC(_IOC_READ,C2_IOC_MAGIC,5,4)
#define C2_IOCQRESET _IO(C2_IOC_MAGIC, 6)
#define C2_IOCIRQCHK _IO(C2_IOC_MAGIC, 7)
#define C2_IOC_MAXNR (7)
