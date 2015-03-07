#define K_PE33_LEN		1024

#define K_DSPIC33_PE_VERSION 0x0026
#define K_DSPIC33_PE_ID	     0x00CB

class CdsP33_PE
{
	public:
		CdsP33_PE(void);
		~CdsP33_PE(void);

		static const unsigned int dsPIC33_PE_Code[K_PE33_LEN];
		static const unsigned int CRC_LUT_Array[256];
};

