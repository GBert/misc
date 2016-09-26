#ifndef CFG_INTERN_H
#define CFG_INTERN_H


#define INI_STR_PORT      "port"
#define INI_STR_UDP_BC    "udpbc"
#define INI_STR_ADDRESS   "address"
#define INI_STR_IFACE     "interface"
#define INI_STR_CAN_IFACE "can_interface"
#define INI_STR_CS2_PATH  "cs2_path"
#define INI_STR_BC        "broadcast"
#define INI_STR_FORK      "fork"
#define INI_STR_TRACE     "trace"
#define INI_STR_VERBOSE   "verbose"
#define INI_STR_USAGE     "usage"
#define INI_STR_ZENTRALE  "zentrale"

#define INI_TOK_PORT      INI_PARS_FIRST_KEYWORD_TOK
#define INI_TOK_UDP_BC    (INI_TOK_PORT+1)
#define INI_TOK_ADDR      (INI_TOK_PORT+2)
#define INI_TOK_IFACE     (INI_TOK_PORT+3)
#define INI_TOK_CAN_IFACE (INI_TOK_PORT+4)
#define INI_TOK_CS2_PATH  (INI_TOK_PORT+5)
#define INI_TOK_ZENTRALE  (INI_TOK_PORT+6)


#endif
