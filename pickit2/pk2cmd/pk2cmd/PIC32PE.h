#define	K_P32_PROGRAM_FLASH_START_ADDR 0x1D000000
#define	K_P32_BOOT_FLASH_START_ADDR    0x1FC00000

#define	K_PE_LOADER_LEN	42
#define K_PE_LEN_RIPE06	1231
#define K_PE_LEN_RIPE11	422
#define K_PE_LEN_RIPE15	1051

#define K_PIC32_PE_VERSION_RIPE06	0x0109
#define K_PIC32_PE_VERSION_RIPE11	0x0301
#define K_PIC32_PE_VERSION_RIPE15	0x0502

class CPIC32PE {
public:
    CPIC32PE(void);
    ~CPIC32PE(void);

    static const unsigned int pe_Loader[K_PE_LOADER_LEN];	//PIC32
    static const unsigned int PIC32_PE_RIPE06[K_PE_LEN_RIPE06];	//PIC32
    static const unsigned int PIC32_PE_RIPE11[K_PE_LEN_RIPE11];	//PIC32
    static const unsigned int PIC32_PE_RIPE15[K_PE_LEN_RIPE15];	//PIC32
};
