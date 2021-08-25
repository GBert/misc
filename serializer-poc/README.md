Bitstream generator (aka serializer) for Ralink RT5350
------------------------------------------------------

Proof of Concept - Bit Stream Serializer to generate bit streams defined thru clockrate and bits via I2S

Ralink low cost WLAN SoC RT5350 provides an I2S interface which could be used to generate bitstream. Because
of the wide range of frequency (20MHz down to 20Khz) and the 32 byte FIFO the I2S offers an
interesting field for experiments ...

The code uses Zilvinas Valinskas mmio tool as base to manipulate the I2S registers.

