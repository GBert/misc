/*
  Written by Wojciech M. Zabolotny (wzab@ise.pw.edu.pl)
  Published under the GPL (Gnu Public License) version 2.0 or later

  This file implements the hardware dependent functions or macros
  to be used with my cygnal_c2 driver

  Currently I assume working directly with the LPT port.
  Well, if someone writes the parport aware version, it would be nice.

  Please do not try to use the USB->LPT converters, as latency is too high
  to generate C2CK low pulses!!!

  Now we assume the following signals assignment:
  D6 - ~C2D_ENA
  D7 - C2D_OUT
  D5 - C2CLK_DATA
  D4 - C2CK_ENA
  BUSY - C2D_IN

 */

/* Below we define the bitmasks for changing the state of particular lines */
#define C2CK_ENA_MASK (0x10)
#define C2CK_MASK (0x20)
#define C2D_MASK (0x80)
#define C2D_ENA_MASK (0x40)

static unsigned char dshadow=0x0; /* Shadow register - keeps the last byte written
				     to the printer data port */

static inline void C2CK_ENA(void)
{
    outb(dshadow &= ~C2CK_ENA_MASK, io_base);
}

static inline void C2CK_DIS(void)
{
    outb(dshadow |= C2CK_ENA_MASK, io_base);
}

static inline void C2CK_SET(int val)
{
  if(val) {
    outb(dshadow |= C2CK_MASK, io_base);
  } else {
    outb(dshadow &= ~C2CK_MASK, io_base);
  }
}

static inline void C2CK_HIGH(void)
{
  C2CK_SET(1);
}

static inline void C2CK_LOW(void)
{
  C2CK_SET(0);
}


static inline int C2D_READ(void)
{
  if(inb(io_base+1) & 0x80) {
    //printk("<1>C2D R0\n");
    return 0;
  } else {
    //printk("<1>C2D R1\n");
    return 1;
  }
}

static inline void C2D_ENA(void)
{
  outb(dshadow &= ~C2D_ENA_MASK, io_base);  
}
// enables the C2D line

static inline void C2D_DIS(void)
{
  outb(dshadow |= C2D_ENA_MASK, io_base);  
}
//disables the C2D line

static inline void C2D_SET(int val)
{
  if(val) {
    outb(dshadow |= C2D_MASK, io_base);

  } else {
    outb(dshadow &= ~C2D_MASK, io_base);
  }
}

static inline void C2D_SETL(void)
{
  C2D_SET(0);
}

static inline void C2D_SETH(void)
{
  C2D_SET(1);
}

static inline void C2D_SETL_EN(void)
{
  C2D_SETL();
  C2D_ENA();
} // - sets low and enables

static inline void C2D_SETH_EN(void)
{
  C2D_SETH();
  C2D_ENA();
} // - sets high and enables

static inline void C2CK_ENA_IRQ(void)
{
  outb(inb(io_base+2) | 0x10, io_base+2);  
}

static inline void C2CK_DIS_IRQ(void)
{
  outb(inb(io_base+2) &~0x10, io_base+2);  
}

