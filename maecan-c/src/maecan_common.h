/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ixam97@ixam97.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * Maximilian Goldschmidt
 * ----------------------------------------------------------------------------
 * https://github.com/Ixam97
 * ----------------------------------------------------------------------------
 */

#ifndef _MAECAN_COMMON
#define _MAECAN_COMMON

#include <stdlib.h>
#include <stdint.h>

/* Märklin CAN-Bus commands: */
#define SYS_CMD             0x00
/* Sys-Subcmds */
    #define SYS_STOP        0x00
    #define SYS_GO          0x01
    #define SYS_HALT        0x02
    #define SYS_LOCO_HALT   0x03
    #define SYS_CYCLE_STOP  0x04
    #define SYS_LOCO_PROTO  0x05
    #define SYS_SWITCHTIME  0x06
    #define SYS_MFX_FAST    0x07
    #define SYS_TRACK_PROTO 0x08
    #define SYS_SET_NAZ     0x09
    #define SYS_OVERLOAD    0x0A
    #define SYS_STATUS      0x0B
    #define SYS_IDENTIFYER  0x0C
    #define SYS_MFX_SEEK    0x30
    #define SYS_RESET       0x80
#define LOCO_DISCOVERY      0x01
#define MFX_BIND            0x02
#define MFX_VERIFY          0x03
#define LOCO_SPEED          0x04
#define LOCO_DIR            0x05
#define LOCO_FN             0x06
#define READ_CONFIG         0x07
#define WRITE_CONFIG        0x08
#define SWITCH_ACC          0x0B
#define CONFIG_ACC          0x0C
#define S88_POLL            0x10
#define S88_EVENT           0x11
#define SX1_EVENT           0x12
#define PING                0x18
#define UPDATE_OFFER        0x19
#define READ_CONFIG_DATA    0x1A
#define BOOTLOADER_CAN      0x1B
#define BOOTLOADER_TRACK    0x1C
#define STATUS_CONFIG       0x1D
#define DATA_QUERY          0x20
#define CONFIG_DATA_STREAM  0x21

struct reading_t {
    /* Describing a reading channel of a device */
    uint8_t index;
    int8_t power;
    char colors[4][17];
    uint16_t origin;
    uint16_t ranges[4];
    char *name;
    char *start;
    char *end;
    char *unit;
};

struct config_dropdown_t {
    /* Describing a dropdown config channel of a device */
    uint8_t index;
    uint8_t n_options;
    uint8_t def_option;
    char *description;
    char **options;
};

struct config_slider_t {
    /* Describing a slider config channel of a device */
    uint8_t index;
    uint16_t min;
    uint16_t max;
    uint16_t def_value;
    char *description;
    char *start;
    char *end;
    char *unit;
};

union config_t {
    struct config_dropdown_t dropdown;
    struct config_slider_t slider;
};

struct device_t {
    /* Describing a CAN-bus device */
    uint32_t uid;
    uint8_t v_low;
    uint8_t v_high;
    uint16_t type;
    uint8_t n_reads;
    uint8_t n_configs;
    uint32_t serial_nbr;
    char product_nbr[8];
    char *name;
    struct reading_t *readings;
    uint8_t *config_types;
    union config_t *configs;
};

struct loco_function_t{
    uint16_t type;
    uint8_t time;
    uint8_t value;
};

struct loco_info_t{
    uint16_t uid;
    char *name;
    uint16_t address;
    uint8_t type; /* 0: mm2_prg, 1: mm2_dil8, 2: dcc, 3: mfx, 4: sx1 */
    uint32_t sid;
    uint32_t mfxuid;
    char *icon;
    uint8_t symbol; /* 0: E-Lok, 1: Diesellok, 2: Dampflok, 3: -/- */
    uint8_t av;
    uint8_t bv;
    uint8_t volume;
    uint16_t tachomax;
    uint8_t vmax;
    uint8_t vmin;
    /* loco_function_t functions[16]; */
};

#endif