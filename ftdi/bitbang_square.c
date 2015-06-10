
/* compile with 
  gcc -o bitbang_square bitbang_square.c -lftdi
*/

#include <stdio.h>
#include <string.h>
#include <ftdi.h>
 
int main()
{
  struct ftdi_context ftdic;
  ftdi_init(&ftdic);
  int i;
 
  if(ftdi_usb_open_desc(&ftdic, 0x0403, 0x6001, NULL, NULL) < 0) {
    fprintf(stderr, "ftdi_usb_open_desc failed: %s\n", 
      ftdi_get_error_string(&ftdic));
    exit(1);
  }
 
  if(ftdi_set_baudrate(&ftdic, 10000) < 0) {
    fprintf(stderr, "ftdi_set_baudrate failed: %s\n", 
      ftdi_get_error_string(&ftdic));
    exit(1);
  }
 
  /* if(ftdi_set_bitmode(&ftdic, 0xff, BITMODE_SYNCBB) < 0) { */
  if(ftdi_set_bitmode(&ftdic, 0xff, BITMODE_BITBANG) < 0) {
    fprintf(stderr, "ftdi_set_bitmode failed: %s\n", 
      ftdi_get_error_string(&ftdic));
    exit(1);
  }
 
  uint8_t data[256];
  for(i=0; i<sizeof(data); i++)
    if (i & 0x01)
	data[i]=0x00;
    else
	data[i]=0xff;
 
  for(;;) {
    ftdi_write_data(&ftdic, data, sizeof(data));
#if 0
    uint8_t data2[256];
    ftdi_read_data(&ftdic, data2, sizeof(data2));
#endif
  }
  /* TODO : never reached */
  if(ftdi_disable_bitbang(&ftdic)) {
    fprintf(stderr, "ftdi_set_bitmode failed: %s\n", 
      ftdi_get_error_string(&ftdic));
    exit(1);
  }
 
  return 0;
}
