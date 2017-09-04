------------------------------------------------------------------------------
README
------------------------------------------------------------------------------

current status:
f1: frootloop3 gets corrupt data in rx path, _after_ serial rx,
but the usb in packets (wireshark) show the corruption. (LA on serial lines is clean)
f4: frootloop3 tests clean
=> suspect libopecm3 usb bug?

frootloop3: github.com/karlp/frootloop3



This implements a USB CDC-ACM device, connected to USART2 (PA2-tx/PA3-rx)
An interrupt driven tx ring buffer is used for usb->serial, and a similar
interrupt driven rx ring buffer is used for serial->usb.
Baud rates from 2400-460800 are tested with zmodem transfers in both directions.

Parity not yet finished, and baudrates below 1200 need fiddling with clock prescalers.
When it's finished, rs485 support should be available via the TC interrupt.

A GPIO is toggled around USART transmissions to control an RS485 line
transceiver, following ST's app note. The RS485 driver enable line is,
by default, just the onboard red LEDs.

The orange/blue LEDs indicate tx/rx activity, but especially for the rx line,
you won't really see it without heavy data rates.

This example is heavily based on the existing usb_cdcacm and usart demos.


KARL - check https://github.com/dhylands/libopencm3-usb-serial/blob/master/usb.c to see if it passes the same tests or not
