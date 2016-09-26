/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef __MAERKLIN_H__
#define __MAERKLIN_H__

/* MS1/CS1 to MS2/CS2 differentiating */
#define M_CS2_HASH_MASK   0x00000380U
#define M_CS2_HASH_FLAG   0x00000300U

/* Device IDs CAN ping
 * Teilnehmer Ping
 */
#define M_DEVICE_ID_GFP         0x0000
#define M_DEVICE_ID_60112       0x0010
#define M_DEVICE_ID_60113       0x0010
#define M_DEVICE_ID_6021        0x0020
#define M_DEVICE_ID_60128       0x0020
#define M_DEVICE_ID_MS2         0x0030
#define M_DEVICE_ID_IPAD        0xEEEE  /* seen on capture - undocumented */
#define M_DEVICE_ID_WD          0xFFE0
#define M_DEVICE_ID_CS2GUI      0xFFFF

/* GBox Magic Start sequence
 * CAN_EFF 00 36 03 01 [5] 00 00 00 00 11
 */
const uint8_t M_GLEISBOX_MAGIC_START_SEQUENCE_LENGTH = 10;
const uint8_t M_GLEISBOX_MAGIC_START_SEQUENCE [] = {0x00,0x36,0x03,0x01,0x05,0x00,0x00,0x00,0x00,0x11};

typedef enum {MS1_START=0, MS1_STAGE1, MS1_STAGE2, MS1_STAGE3, MS1_STAGE4,
              MS1_STAGE5, MS1_STAGE6, MS1_STAGE7, MS1_LOCKED} state_t;

/* MS1/CS1 specifc parts */
uint8_t ms1_stage1_msg[8] = { 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01 };
uint8_t ms1_stage2_msg[8] = { 0x00, 0x00, 0x00, 0x97, 0x01, 0x00, 0xc0, 0x10 };
uint8_t ms1_stage3_msg[8] = { 0x01, 0x00, 0x00, 0x69, 0x00, 0x00, 0x40, 0x10 };
uint8_t ms1_stage4_msg[8] = { 0x00, 0x00, 0x40, 0x10, 0x01, 0x03, 0x00, 0x84 };
uint8_t ms1_stage7_msg[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00 };

struct ms1_fsm_state_structure {
    uint8_t id;
    uint8_t next_state;
    uint8_t switch_state;
    uint8_t *can_message;
    char *name;
    char *debug_message;
};

struct ms1_fsm_state_structure my_ms1_fsm_state[] = {
    { 0, MS1_STAGE1, 0, &ms1_stage1_msg[0], "Stufe 0 Quittung", "found MS1 slave device" },
    { 0, MS1_STAGE2, 1, &ms1_stage2_msg[0], "Stufe 1 Quittung", "Stufe 2" },
    { 0, MS1_STAGE3, 2, &ms1_stage3_msg[0], "Stufe 2 Quittung", "Stufe 3" },
    { 0, MS1_STAGE4, 3, &ms1_stage4_msg[0], "Stufe 3 Quittung", "Stufe 4" },
    { 0, MS1_STAGE5, 4, NULL              , "Stufe 4 Quittung", "Stufe 5" },
    { 0, MS1_STAGE6, 5, NULL              , "Stufe 5 Quittung", "Stufe 6" },
    { 0, MS1_STAGE7, 6, NULL              , "Stufe 6 Quittung", "Stufe 7" },
    { 0, MS1_LOCKED, 7, &ms1_stage7_msg[0], "Stufe 7 Quittung", "Stufe 7" },
};

struct ms1_handle {
    uint8_t  prio;
    uint8_t  UID;
    uint8_t  stage;
    uint8_t  MID;
    uint8_t  direction;
    uint8_t  command;
    uint16_t object_handle;
    uint8_t  node;
};

struct ms1_id {
    uint8_t ms1_state;
    uint8_t Master_UID[4];
    uint8_t Slave_UID[4];
    struct ms1_fsm_state *ms1_fsm_state;
};

#endif
