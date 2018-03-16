
/*
lok
.uid=0x4005
.name=M4
.adresse=0x9
.typ=mfx
.mfxuid=0xff001234
.av=64
.bv=48
.volume=64
.vmax=255
.vmin=12
.fkt
..nr=0
..typ=32
.mfxAdr
..target=2
..name=3
..addr=136
..xcel=78
..speedtable=79
..volume=117
..numfunc=16
..func=21

*/

unsigned char pre_mfx[]   = { 0x02, 0xf5, 0x00 };
unsigned char pre_other[] = { 0x02, 0xc5, 0x00 };

struct mfxAdr_t {
    unsigned int target;
    char *name;
    unsigned int address;
    unsigned int xcel;
    unsigned int speedtable;
    unsigned int volume;
    unsigned int numfunc;
    unsigned int funcy;
};

struct loco_config_t {
    int eeprom_max_size;
    unsigned int eeprom_size;
    int id;
    FILE *fp;
    char *filename;
    unsigned char *bin;
};

struct loco_data_t {
    unsigned int long_uid;
    unsigned int uid;
    char *name;
    char *proto;
    unsigned int address;
    unsigned int typ;
    unsigned int mfxuid;
    unsigned int acc_delay;		/* av */
    unsigned int slow_down_delay;	/* bv */
    unsigned int volume;
    unsigned int vmax;
    unsigned int vmin;
    struct mfxAdr_t mfxAdr;
};
