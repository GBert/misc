#include <stdio.h>
#include <string.h>
#include <ftdi.h>
 
int main()
{
  struct ftdi_context *ftdic;
  uint8_t data[256];
  int i;

  if ((ftdic = ftdi_new()) == 0) {
    fprintf(stderr, "ftdi_new failed\n");
    exit(1);
  }

  if(ftdi_usb_open_desc(ftdic, 0x0403, 0x6015, NULL, NULL) < 0) {
    fprintf(stderr, "ftdi_usb_open_desc failed: %s\n", ftdi_get_error_string(ftdic));
    exit(1);
  }
 
  /* if(ftdi_set_bitmode(ftdic, 0xff, BITMODE_SYNCBB) < 0) { */
  if(ftdi_set_bitmode(ftdic, 0xff, BITMODE_BITBANG) < 0) {
    fprintf(stderr, "ftdi_set_bitmode failed: %s\n", ftdi_get_error_string(ftdic));
    exit(1);
  }

  /* multiply 64 ? */
  /* 269 -> DCC 58us */
  /* 601 -> MM  26us */
  /* 301 -> MM  52us */
  /* 312 -> mfx 50us */

  if(ftdi_set_baudrate(ftdic, 269) < 0) {
    fprintf(stderr, "ftdi_set_baudrate failed: %s\n", ftdi_get_error_string(ftdic));
    exit(1);
  }

  for(i=0; i<sizeof(data); i++)
    data[i]= i & 0x0f;
 
  for(;;) {
    ftdi_write_data(ftdic, data, sizeof(data));
#if 1
    uint8_t data2[256];
    ftdi_read_data(ftdic, data2, sizeof(data2));
#endif
  }
  /* TODO : never reached */
  if(ftdi_disable_bitbang(ftdic)) {
    fprintf(stderr, "ftdi_set_bitmode failed: %s\n", ftdi_get_error_string(ftdic));
    exit(1);
  }
 
  return 0;
}
