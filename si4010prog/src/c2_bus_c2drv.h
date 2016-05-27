/**
 * c2_bus_c2drv.h - SiLabs C2 master bus interface using Linux kernel module
 *
 * Based on code written by Wojciech M. Zabolotny (wzab@ise.pw.edu.pl)
 * and is published under the GPL (Gnu Public License) version 2.0 or later
 */
#ifndef __C2_BUS_C2DRV_H__
#define __C2_BUS_C2DRV_H__

int c2_bus_c2drv_init(struct c2_bus *bus, const char *path);

#endif // __C2_BUS_C2DRV_H__
