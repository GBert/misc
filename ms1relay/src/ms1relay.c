// ms1relay.c : MS1 adaption to MCS
//
// C 2020-2021 Rainer Müller
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).
// Teile übernommen aus SW von Gerhard Bertelsmann

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <netinet/in.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>

#include "ms1relay.h"

#define VPRT if (verbose) printf

#define INVALID_SOCKET -1
static int sc = INVALID_SOCKET;

#define DEF_CAN "can0"
#define DEF_LOCODB "/www/config/lokomotive.cs2"

static unsigned char stage0reply[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, OH_PING};
static unsigned char stage7reply[4] = {0x00, OH_ROOT, 0x01, 0x00};
static unsigned char sdhandlerep[6] = {0x00, 0x00, 0x00, OH_SDES, 0x00, 0x01};
static unsigned char syhandlerep[6] = {0x00, 0x00, 0x00, OH_SYST, 0x00, 0x01};
static unsigned char querymsname[3] = {0x02, 0x00, 0x00};
static unsigned char allowMScntl[6] = {0x03, 0x00, 0x00, 0x08, 0x00, 0x01};
static unsigned char locostackad[8] = {0x80, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03};
static unsigned char locostackrm[8] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

// device specific data in NW order <------- UID ------->  <- vers ->  < device >
static unsigned char M_DEV_ID[] = {0x76, 0x54, 0x4D, 0x01, 0x15, 0x03, 0xFF, 0xF1};

// device description (index 0)
static unsigned char M_DEV_STAT[][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {"04731\0\0"},
    {0x6D, 0x73, 0x31, 0x72, 0x65, 0x6C, 0x61, 0x79},  // ms1relay
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   };

ms1dat_t ms1d;
int     powsync, verbose;
int     querystep = 0;
char    locodbname[128];

uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

void fillid(uint8_t *u, uint32_t v)
{
    u[0] = v >> 24; u[1] = v >> 16; u[2] = v >> 8;  u[3] = v;
}

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version %u.%u\n\n", M_DEV_ID[4], M_DEV_ID[5]);
    fprintf(stderr, "         -i <can int>        can interface - default %s\n", DEF_CAN);
    fprintf(stderr, "         -d                  daemonize\n");
    fprintf(stderr, "         -l <loco_db>        loco database file - default %s\n", DEF_LOCODB);
    fprintf(stderr, "         -p                  power state synchronisation\n");
    fprintf(stderr, "         -v                  verbose output\n\n");
}

void locostackAdd(uint8_t node, uint16_t objhandle)
{
    struct can_frame frame;
    frame.can_id = 0x18000200 | CAN_EFF_FLAG | (OH_ROOT << 10) | node;
    frame.can_dlc = 8;
    memcpy(frame.data, locostackad, 8);
    frame.data[4] = objhandle >> 8;
    frame.data[5] = objhandle & 0xFF;
    if (write(sc, &frame, sizeof(struct can_frame)) < 0)
        fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
    ms1d.multiadd++;
    ms1d.slvstackcnt++;
    VPRT("Add to stack of node %u OH %u, count is now %u\n", node, objhandle, ms1d.slvstackcnt);
}

void locostackRemove(uint8_t node, uint16_t objhandle)
{
    struct can_frame frame;
    frame.can_id = 0x18000280 | CAN_EFF_FLAG | (OH_ROOT << 10) | node;
    frame.can_dlc = 8;
    memcpy(frame.data, locostackrm, 8);
    frame.data[4] = objhandle >> 8;
    frame.data[5] = objhandle & 0xFF;
    if (write(sc, &frame, sizeof(struct can_frame)) < 0)
        fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
    ms1d.slvstackcnt--;
    VPRT("Remove from stack of node %u OH %u, count is now %u\n", node, objhandle, ms1d.slvstackcnt);
}

void queryPowerState()
{
    struct can_frame frame;
    frame.can_id = 0x004711 | CAN_EFF_FLAG;
    frame.can_dlc = 4;
    memset(frame.data, 0, 4);
    if (write(sc, &frame, sizeof(struct can_frame)) < 0)
        fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
}

// send data blocks via CAN
static void send_datablock(unsigned char (*info)[8], int nmbr)
{
    struct can_frame frame;
    frame.can_dlc = 8;

    for (int pkts = 0; pkts < nmbr; pkts++) {
        memcpy(frame.data, info[pkts], 8);
        frame.can_id = (0x003B0301 + pkts) | CAN_EFF_FLAG;
        // send data block frame
        if (write(sc, &frame, sizeof(struct can_frame)) < 0)
            fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
    }
}

void queryLocoState()
{
    struct can_frame frame;
    switch (querystep) {
        case 1:     frame.can_id = 0x084711 | CAN_EFF_FLAG;     // speed
                    frame.can_dlc = 4;
                    break;
        case 2:     frame.can_id = 0x0A4711 | CAN_EFF_FLAG;     // direction
                    frame.can_dlc = 4;
                    break;
        default:    frame.can_id = 0x0C4711 | CAN_EFF_FLAG;     // function
                    frame.can_dlc = 5;
                    frame.data[4] = querystep - 3;
                    break;
    }
    fillid(frame.data, ms1d.slvassignedLID);
    if (write(sc, &frame, sizeof(struct can_frame)) < 0)
        fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
    querystep--;
}

void sendstatetoall(struct can_frame frame)
{
    uint8_t node = 0;       // TODO: loop over all MS1, but actually there is only one
    frame.data[1] = 0;
    frame.can_dlc = 3;
    frame.can_id = 0x10000100 | CAN_EFF_FLAG | (OH_SYST << 10) | node;
    if (write(sc, &frame, sizeof(struct can_frame)) < 0)
        fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
}

void handleStdCAN(struct can_frame *frame)
{
    uint16_t v;
    switch ((frame->can_id & 0x01FF0000UL) >> 16) {
        // System commands
        case 0x01:  switch (frame->data[4]) {
                // Stop
                case 0x00:  frame->data[0] = 1;
                            frame->data[2] = 1;     // show STOP
                            sendstatetoall(*frame);
                            VPRT("Sending STOP to all MS1 nodes\n");
                            break;
                // Go
                case 0x01:  frame->data[0] = 1;
                            frame->data[2] = 0;     // clear STOP
                            sendstatetoall(*frame);
                            frame->data[0] = 2;
                            frame->data[2] = 0;     // clear OVERLOAD
                            sendstatetoall(*frame);
                            VPRT("Sending GO to all MS1 nodes\n");
                            break;
                // Overload
                case 0x0A:  frame->data[0] = 2;
                            frame->data[2] = 1;     // show OVERLOAD
                            sendstatetoall(*frame);
                            VPRT("Sending OVERLOAD to all MS1 nodes\n");
                            break;
                // unhandled codes
//              default:
                }
                frame->can_id = 0;      // no reply
                break;
        // loco speed
        case 0x09:  if (be16(&frame->data[2]) == ms1d.slvassignedLID) {
                        v = be16(&frame->data[4]) >> 3;
                        if (ms1d.direction) v += 0x80;
                        frame->data[0] = 1;
                        frame->data[1] = 0;
                        frame->data[2] = v;
                        frame->can_dlc = 3;
                        frame->can_id = 0x18000100 | CAN_EFF_FLAG | (ms1d.slvassignedOH << 10);
                        VPRT("Change of FKT %u to %u\n", frame->data[0], frame->data[2]);
                    }
                    else frame->can_id = 0; // no reply
                    break;
        // loco direction
        case 0x0B:  if (be16(&frame->data[2]) == ms1d.slvassignedLID) {
                        ms1d.direction = frame->data[4] - 1;
                    }
                    frame->can_id = 0;      // no reply
                    break;
        // loco function
        case 0x0D:  if (be16(&frame->data[2]) == ms1d.slvassignedLID) {
                        frame->data[0] = frame->data[4] + 2;
                        frame->data[1] = 0;
                        frame->data[2] = frame->data[5];
                        frame->can_dlc = 3;
                        frame->can_id = 0x18000100 | CAN_EFF_FLAG | (ms1d.slvassignedOH << 10);
                        VPRT("Change of FKT %u to %u\n", frame->data[0], frame->data[2]);
                    }
                    else frame->can_id = 0; // no reply
                    break;
        // Ping
        case 0x30:  frame->can_id += 0x10000;
                    frame->can_dlc = 8;
                    memcpy(frame->data, M_DEV_ID, 8);
                    break;
        // status data config
        case 0x3A:  if ((memcmp(frame->data, M_DEV_ID, 4)) || (frame->data[4]))
                        frame->can_id = 0;  // no reply
                    else {
                        send_datablock(M_DEV_STAT, sizeof(M_DEV_STAT)/8);
                        frame->can_id += 0x10000;
                        frame->can_dlc = 6;
                        frame->data[5] = sizeof(M_DEV_STAT)/8;
                    }
                    break;
        // unhandled codes
        default:    frame->can_id = 0;      // no reply
    }
}

void queryName(unsigned char node, unsigned char start)
{
    struct can_frame frame;
    frame.can_id = 0x18000080 | CAN_EFF_FLAG | (OH_SDES << 10) | node;
    frame.can_dlc = 4;
    memcpy(frame.data, querymsname, 3);
    frame.data[3] = start;
    if (write(sc, &frame, sizeof(struct can_frame)) < 0)
        fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
}

int main(int argc, char **argv) {
    int max_fds, opt;
    struct can_frame frame;

    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set read_fds;

    int background = 0;
    verbose = 0;
    powsync = 0;
    strcpy(ifr.ifr_name, DEF_CAN);
    strcpy(locodbname, DEF_LOCODB);

    while ((opt = getopt(argc, argv, "i:l:dpvh?")) != -1) {
    switch (opt) {
    case 'i':
        strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
        break;
    case 'd':
        verbose = 0;
        background = 1;
        break;
    case 'l':
        strncpy(locodbname, optarg, sizeof(locodbname) - 1);
        break;
    case 'p':
        powsync = 1;
        break;
    case 'v':
        verbose = 1;
        break;
    case 'h':
    case '?':
        print_usage(basename(argv[0]));
        exit(EXIT_SUCCESS);
        break;
    default:
        fprintf(stderr, "Unknown option %c\n", opt);
        print_usage(basename(argv[0]));
        exit(EXIT_FAILURE);
    }
    }

    if (readDBfile(locodbname, 10, verbose)) exit(EXIT_FAILURE);

    memset(&caddr, 0, sizeof(caddr));

    /* prepare CAN socket */
    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
    fprintf(stderr, "error creating CAN socket: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
    }
    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
    fprintf(stderr, "setup CAN socket error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
    fprintf(stderr, "error binding CAN socket: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
    }

    /* daemonize the process if requested */
    if (background) {
    if (daemon(0, 0) < 0) {
        fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    }

    FD_ZERO(&read_fds);
    FD_SET(sc, &read_fds);
    max_fds = sc;

    while (1) {
    if (select(max_fds + 1, &read_fds, NULL, NULL, NULL) < 0) {
        fprintf(stderr, "select error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* received a CAN frame */
    if (FD_ISSET(sc, &read_fds)) {
        if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
        fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
        }
        else if (frame.can_id & CAN_EFF_FLAG) { /* only EFF frames are valid */

/*  ID field for MS1, coding for normal operation and for detection:
    28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    <-PRIO-> <------          OBJECTHANDLE           ------> <-CMND-> <---    NODE    --->
    <-PRIO-> <---      UID      ---> <-STEP-> <--  MID   --> <-CMND-> <---    NODE    --->
*/
    uint8_t   mid;
    uint8_t   stage;
    uint16_t  lid;
    uint16_t  objhandle = (frame.can_id >> 10) & 0xffff;
    uint8_t   node = frame.can_id & 0x7f;
    locodat_t *locodatptr = (objhandle >= OH_LOCO) ? getlocodat(objhandle) : NULL;

    switch (frame.can_id & 0x1C000380) {
    case 0x00000300:        // Prio 000, Cmd 110    => MCS CAN commands
        handleStdCAN(&frame);
        break;
    case 0x08000100:        // Prio 010, Cmd 010    => events from MS1
        frame.can_id = 0;       // no reply
        if (objhandle == OH_SYST) {
            printf("Node %u reports change of system FKT %u to %u\n", node, frame.data[0], frame.data[2]);
            if (frame.data[0] == 1) {
                frame.can_id = 0x004711 | CAN_EFF_FLAG;
                frame.can_dlc = 5;
                frame.data[4] = 1 - (frame.data[2] & 1);    // stop or go
                memset(frame.data, 0, 4);
            }
        }
        else if (locodatptr) {
            lid = locodatptr->locoid;
            if (lid == 0) frame.data[0] = 0;                    // dummy loco
            VPRT("Node %u reports change of FKT %u to %u\n", node, frame.data[0], frame.data[2]);
            if (frame.data[0] == 1) {
                frame.can_id = 0x084711 | CAN_EFF_FLAG;         // speed
                frame.can_dlc = 6;
                frame.data[4] = (frame.data[2] & 0x7F) >> 5;
                frame.data[5] = frame.data[2] << 3;
                if ((frame.data[2] >> 7) != ms1d.direction) {
                    ms1d.direction = frame.data[2] >> 7;
                    fillid(frame.data, lid);
                    if (write(sc, &frame, sizeof(struct can_frame)) < 0)
                        fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
                    frame.can_id = 0x0A4711 | CAN_EFF_FLAG;     // direction
                    frame.can_dlc = 5;
                    frame.data[4] = ms1d.direction + 1;
                }
                fillid(frame.data, lid);
            }
            else if (frame.data[0] > 1) {
                frame.can_id = 0x0C4711 | CAN_EFF_FLAG;         // function
                frame.can_dlc = 6;
                frame.data[4] = frame.data[0] - 2;
                frame.data[5] = frame.data[2];
                fillid(frame.data, lid);
            }
        }
        break;
    case 0x0C000380:        // Prio 011, Cmd 111    => periodic supervision
        frame.can_id--;
        frame.can_dlc = 8;
        memcpy(frame.data+4, ms1d.slaveuid, 4);
        frame.data[0] = 3;
        VPRT("Node %u gets Ping Answer from master node %u\n", node, frame.data[3]);
        break;
    case 0x14000000:        // Prio 101, Cmd 000    => control take-over
        frame.can_id--;
        if (locodatptr) {
            ms1d.slvassignedOH = objhandle;
            ms1d.slvassignedLID = locodatptr->locoid;
            if (locodatptr->locoid) querystep = locodatptr->funmbr + 2;
        }
        VPRT("Node %u has selected loco with handle %u and ID %u\n", node, objhandle, ms1d.slvassignedLID);
        break;
    case 0x18000000:        // Prio 110, Cmd 000    => allow MS1 to control
        frame.can_id--;
        frame.can_dlc = 6;
        memcpy(frame.data, allowMScntl, 6);
        VPRT("Node %u allows control loco with handle %u\n", node, objhandle);
        if (ms1d.multiadd) {
            objhandle = getNextOH(node, objhandle);
            if (objhandle) {
                locostackAdd(node - 1, objhandle);
            }
            else ms1d.multiadd = 0;
        }
        break;
    case 0x18000080:        // Prio 110, Cmd 001    => device and loco properties
        if (objhandle == OH_SDES) {
            if (frame.data[3] <= 12) memcpy(ms1d.slvname+frame.data[3], frame.data+4, 4);
            if (frame.data[3] <= 8) queryName(node-1, frame.data[3]+4);
            else
                printf("Node %u told its name %s\n", node, ms1d.slvname);
            frame.can_id = 0;
        }
        else {
            switch (frame.data[0]) {
            case 0x02:  frame.can_id--;     // loco name
                        frame.can_dlc = 8;
                        if ((locodatptr != NULL) && (frame.data[3] <= 12))
                            memcpy(frame.data+4, &locodatptr->loconame[frame.data[3]], 4);
                        else
                            memset(frame.data+4, frame.data[3] ? 0 : '?', 4);
                        break;
            case 0x40:  switch (frame.data[1]) {
                case 1: frame.can_id--;     // rail signal format
                        frame.can_dlc = 8;
                        memset(frame.data+4, 0, 4);
                        frame.data[5] = 3;  // TODO: insert correct value
                        break;
                case 3: frame.can_id--;     // loco type
                        frame.can_dlc = 6;
                        frame.data[4] = 0;
                        frame.data[5] = (locodatptr) ? locodatptr->locoicon : 0;
                        VPRT("Node %u gets loco info for handle %u\n", node, objhandle);
                        break;
                case 4: frame.can_id--;     // TODO: clarify whats that
                        frame.can_dlc = 6;
                        frame.data[4] = 1;
                        frame.data[5] = 0;
                        break;
                }
                break;
            default:    frame.can_id = 0;
            }
        }
        break;
    case 0x18000100:        // Prio 110, Cmd 010    => device and loco states
        if (objhandle == OH_SYST) {
            frame.can_id--;
            switch (frame.data[0]) {
            case 1:                                 // Stop / Go
            case 2: frame.data[2] = frame.data[0];  // overload
                    frame.can_dlc = 3;
                    break;
            case 3: frame.data[2] = 4;              // mfx
                    frame.can_dlc = 3;
                    break;
            case 4: if (powsync)
                        queryPowerState();  // trigger power state synchronisation
                    break;
            }
        }
        else {
            switch (frame.data[1]) {
            case 0: frame.can_id--;     // function value
                    frame.can_dlc = 3;
                    frame.data[2] = 0;  // TODO: insert correct value
                    break;
            case 1: frame.can_id--;     // function type
                    if (frame.data[0] == 1) {
                        frame.can_dlc = 4;
                        frame.data[2] = 1;
                        frame.data[3] = 1;
                    } else if ((locodatptr) && (frame.data[0] < (locodatptr->funmbr + 2))) {
                        lid = locodatptr->futype[frame.data[0] - 2];
                        frame.can_dlc = 4;
                        // HACK: workaround because momentary mode does not work like specified
                        frame.data[2] = (lid & 0x80) ? 2 : 2;       // mode
                        // HACK: icon type 0 not accepted, so use 0x10 for no icon
                        frame.data[3] = lid ? (lid & 0x7F) : 0x10;  // icon
                    }
                    break;
            case 2: frame.can_id--;     // ???
                    frame.can_dlc = 5;
                    frame.data[4] = 0x0F;      // TODO: insert correct values
                    break;
            }
        }
        break;
    case 0x18000180:        // Prio 110, Cmd 011    => SYS_OH and loco stack
        switch (frame.data[0]) {
        case 0x03:  frame.can_id--;
                    frame.can_dlc = 8;
                    memcpy(frame.data+2, syhandlerep, 6);
                    printf("Node %u gets SystemHandle %u\n", node, be16(frame.data+4));
                    queryName(node-1, 0);
                    break;
        case 0x80:  frame.can_id--;
                    VPRT("Node %u requests loco stack (above %u)\n", node, be16(frame.data+2));
                    objhandle = getNextOH(node, be16(frame.data+2));
                    if (objhandle) {
                        ms1d.slvstackcnt++;
                        VPRT("Node %u gets loco stack entry %u Handle %u for ID %04X\n",
                                node, ms1d.slvstackcnt, objhandle, getlocodat(objhandle)->locoid);
                        frame.can_dlc = 8;
                        frame.data[4] = objhandle >> 8;
                        frame.data[5] = objhandle & 0xFF;
                        frame.data[6] = 0;
                        frame.data[7] = 1;
                    }
                    break;
        default:    frame.can_id--;
                    printf("Node %u sent unknown Data0 %02X, echoed\n", node, frame.data[0]);
        }
        break;
    case 0x18000200:        // Prio 110, Cmd 100    => Slave Descr. handle
        switch (frame.data[0]) {
        case 0x40:  frame.can_id--;
                    frame.can_dlc = 8;
                    memcpy(frame.data+2, sdhandlerep, 6);
                    printf("Node %u gets SlaveDescriptionHandle %u\n",  node, be16(frame.data+4));
                    break;
        case 0x80:  frame.can_id = 0;      // not yet coded
//                  printf("Lokstack erweitern für Node %u mit Index %u", frame.data[1], be16(frame.data+4));
                    break;
        }
        break;
    case 0x18000280:        // Prio 110, Cmd 101
        if (frame.can_id == (0x180002F0 | CAN_EFF_FLAG)) {
            printf("Updating the loco database.\n");
            updatereadDB(locodbname, 10, verbose);
        }
        frame.can_id = 0;
//      printf("Lokstackeintrag löschen mit Index %u", be16(frame->data + 4));
        break;
    case 0x1C000000:        // Prio 111, Cmd 00X    => MS1 discovery and startup
        stage = (frame.can_id >> 15) & 0x07;
        VPRT("MS1 Identification Stage %u\n", stage);
        switch (stage) {
        case 0: mid = 1;       // HACK: because only 1 MS1 we use always 1
                frame.can_id |= (0x80 | (mid << 10));
                frame.can_dlc = 8;
                memcpy(frame.data, stage0reply, 8);
                break;
        case 4: memcpy(ms1d.slaveuid, frame.data, 4);
                /* fall thru */
        case 5:
        case 6: frame.can_id = 0;      // no reply
                break;
        case 7: if (frame.can_dlc == 8) memcpy(ms1d.slvversion, frame.data+4, 2);
                frame.can_id |= 0x80;
                frame.can_dlc = 8;
                memcpy(frame.data, ms1d.slaveuid, 4);
                memcpy(frame.data+4, stage7reply, 4);
                printf("Node %u has UID %08X and AP Version %u.%u\n",
                    frame.data[6], be32(ms1d.slaveuid), ms1d.slvversion[0], ms1d.slvversion[1]);
                ms1d.slvstackcnt = 0;
                break;
        default:frame.can_id |= 0x80;   // stages 1, 2, and 3
                frame.can_dlc = 8;
                memset(frame.data, 0, 8);
        }
        break;
    default:
        printf("<message %08X is still not decoded>\n", frame.can_id);
        frame.can_id = 0;           // no reply
    }
    if (frame.can_id) {
        if (write(sc, &frame, sizeof(struct can_frame)) < 0)
            fprintf(stderr, "error line %d writing CAN frame: %s\n", __LINE__, strerror(errno));
    if (querystep && (ms1d.multiadd == 0))
        queryLocoState();
    }
    }
    }
    }
    close(sc);
    return 0;
}
