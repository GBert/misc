#include <stdio.h>
#include <ftdi.h>
#include <sys/time.h>

#define RXBUFSZ	16384
#define TXBUFSZ 16384

// BAUDRATE = used baudrate, should be a power of 2
#define BAUDRATE	(65536l)

// PLE = 8x bitrate (this is FTDI specific), note: from the FTDI doc it should be 16xBAUDRATE ????
#define F_SAMPLE	(8*BAUDRATE)

// F_CARRIER = 'carrier' frequency (e.g. 38 kHz)
#define F_CARRIER	38000

// F_TESTSIG = test signal frequency (this is the frequency the carrier will be modulated)
#define F_TESTSIG	500

int main(int argc, char **argv)
{
    int ret;
    struct ftdi_context ftdic;
    ftdi_init(&ftdic);
    unsigned char rxbuf[RXBUFSZ];
    unsigned char txbuf[TXBUFSZ];
    int t,i;
    int carrier_ctr;
    int carrier_val;
    int testsig_ctr;
    int testsig_val;
              
//    if((ret = ftdi_usb_open_desc(&ftdic, 0x0403, 0x6001, NULL, "FTE1G5RU")) < 0) 
//    if((ret = ftdi_usb_open_desc(&ftdic, 0x0403, 0x6001, NULL, "FTE1G72J")) < 0) 
    if((ret = ftdi_usb_open_desc(&ftdic, 0x0403, 0x6001, NULL, NULL)) < 0) 
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
        return EXIT_FAILURE;
    }

    if(ftdi_set_bitmode(&ftdic, 0xff, BITMODE_SYNCBB) < 0) {
	fprintf(stderr, "ftdi_set_bitmode failed: %s\n", 
	ftdi_get_error_string(&ftdic));
	exit(1);
    }

    ret = ftdi_set_baudrate(&ftdic,BAUDRATE);
    if (ret<0)
    {
      fprintf(stderr,"Unable to set required baudrate\n");
      return EXIT_FAILURE;
    }
    
    ret = ftdi_set_latency_timer(&ftdic,255);
    if (ret<0)
    {
      fprintf(stderr,"Unable to set latency timer\n");
      return EXIT_FAILURE;
    }

    ret = ftdi_write_data_set_chunksize(&ftdic,65536);
    if (ret<0)
    {
      fprintf(stderr,"unable to set write data chunk size\n");
      return EXIT_FAILURE;
    }
    ret = ftdi_read_data_set_chunksize(&ftdic,65536);
    if (ret<0)
    {
      fprintf(stderr,"unable to set read data chunk size\n");
      return EXIT_FAILURE;
    }
    
      for (i=0; i<TXBUFSZ; i++)
      {
      
        /*
        txbuf[i]=testsig_val;
        testsig_val=testsig_val?0x0:0xff;
        */
      
        // generate 'carrier'
        carrier_ctr += F_CARRIER*2;
        if (carrier_ctr >= F_SAMPLE)
        {
          carrier_val = (carrier_val == 0) ? 4:0;      
          carrier_ctr -= F_SAMPLE;
        }
        
        // modulate carrier by testsig:
        testsig_ctr += F_TESTSIG*2;
        if (testsig_ctr >= F_SAMPLE)
        {
          testsig_val = (testsig_val == 0) ? 255:0;
          testsig_ctr -= F_SAMPLE;
        }
        
        txbuf[i] = testsig_val & carrier_val;
        txbuf[i] = i & 0xff;
      }
    while (!0)
    {
      ret = ftdi_write_data(&ftdic,txbuf,TXBUFSZ);
    }

    ftdi_usb_close(&ftdic);
    ftdi_deinit(&ftdic);

    return EXIT_SUCCESS;
}

