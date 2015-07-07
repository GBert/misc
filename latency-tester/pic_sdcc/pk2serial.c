/* pk2serial (c) 2009 Tom Schouten

   I hereby grant the rights for any use of this software, given that
   this copyright notice is preserved.

   Connect to PK2 and start a serial console on stdio.  Use this in
   conjunction with socat for pty/socket/... emulation.

*/

#include <usb.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>



/* Program options */
int verbose = 0;
int usec = 100000;
int output_fd = 1;
int input_fd = 0;
int baud_rate = 9600;
int vendor  = 0x04d8;
int product = 0x0033;
int exit_on_eof = 0;
int latency_bytes = 32;
int power_target = 0;    // (1)
int reset_target = 0;

/* (1) Manual says not to power target in UART mode.  I found this to
   work but somehow be less reliable..  Note that VDD does need to be
   connected to target power for proper signal clamping.  (You need -p
   with loopback testing!) */




#define ENDPOINT_IN 0x81
#define ENDPOINT_OUT 0x01
#define BUFFER_SIZE 64
#define TIMEOUT 5000

// PK2 commands
#define GET_STATUS        0xA2
#define EXECUTE_SCRIPT    0xA6
#define CLR_DOWNLOAD_BFR  0xA7
#define CLR_UPLOAD_BFR    0xA9
#define DOWNLOAD_DATA     0xA8
#define UPLOAD_DATA       0xAA
#define ENTER_UART_MODE   0xB3
#define EXIT_UART_MODE    0xB4
#define SET_VDD           0xA0
#define RESET             0xAE
#define END_OF_BUFFER     0xAD

// PK2 script instructions
#define VDD_GND_ON        0xFD
#define VDD_ON            0xFF
#define VDD_GND_OFF       0xFC
#define VDD_OFF           0xFE
#define MCLR_GND_ON       0xF7
#define MCLR_GND_OFF      0xF6
#define SET_ISP_PINS      0xF3


usb_dev_handle *handle = NULL; // only one device per program instance.

#define LOGL(level, ...)   { if (verbose >= level) fprintf(stderr, __VA_ARGS__); }
#define LOG(...)   { LOGL(1, __VA_ARGS__); }

#define LOG_ERROR(...) { fprintf(stderr, __VA_ARGS__); }
#define RAISE_ERROR(...) { LOG_ERROR(__VA_ARGS__); exit(1); }

void log_buf(char *kind, unsigned char *data, int len) {
    int i;
    if (len) {
        LOGL(2, "%s: ", kind);
        for(i=0; i<len; i++) { LOGL(2, "%02X ", data[i]); }
        LOGL(2, "\n");
    }
}

/* lowlevel transfer */
void usb_send_buf(void *buffer) {
    int rv;
    if (BUFFER_SIZE != (rv = usb_interrupt_write(handle, ENDPOINT_OUT, buffer, BUFFER_SIZE, TIMEOUT))) {
        RAISE_ERROR("Invalid size request: %d bytes.\n", rv);
    }
}
void usb_receive_buf(void *buffer) {
    int rv;
    if (BUFFER_SIZE != (rv = usb_interrupt_read(handle, ENDPOINT_IN, buffer, BUFFER_SIZE, TIMEOUT))) {
        RAISE_ERROR("Invalid size response: %d bytes.\n", rv);
    }
}

/* buffer formatting based on data length */
void usb_send(void *data, int len) {
    char buffer[BUFFER_SIZE];
    log_buf("W", data, len);
    memset(buffer, END_OF_BUFFER, BUFFER_SIZE);
    memcpy(buffer, data, len);
    usb_send_buf(buffer);
}
void usb_receive(void *data, int len) {
    char buffer[BUFFER_SIZE];
    usb_receive_buf(buffer);
    memcpy(data, buffer, len);
    log_buf("R", data, len);
}

/* one command cycle */
void usb_call(void *cmd,   int cmd_len,
              void *reply, int reply_len) {
    usb_send(cmd, cmd_len);
    if (reply_len) usb_receive(reply, reply_len); // PK2 doesn't send empty buffers
}

#define TRANSACTION(reply_type, ...) ({                                 \
            char cmd[] = { __VA_ARGS__ };                               \
            reply_type reply;                                           \
            usb_call(cmd, sizeof(cmd), &reply, sizeof(reply_type));     \
            reply; })

#define COMMAND(...) {                          \
        char cmd[] = { __VA_ARGS__ };           \
        usb_call(cmd, sizeof(cmd), NULL, 0); }

#define DEFINE_TRANSACTION(return_type, name, ...) return_type name (void) { \
        return  TRANSACTION(return_type, __VA_ARGS__); }


DEFINE_TRANSACTION(short int, get_status, GET_STATUS)

void set_vdd(float vdd) {
    float tolerance = 0.80;
    float vfault = tolerance * vdd;
    int vddi = ((32.0f * vdd) + 10.5f) * 64.0f;
    int vfi  = (vfault / 5.0) * 255.0f;
    COMMAND(SET_VDD, vddi & 0xFF, (vddi >> 8) & 0xFF, vfi);
}

void reset_pk2(void)     { COMMAND(RESET); }
void reset_hold(int rst) { COMMAND(EXECUTE_SCRIPT, 1, rst ? MCLR_GND_ON : MCLR_GND_OFF); }
void reset_release(void) { COMMAND(EXECUTE_SCRIPT, 1, MCLR_GND_OFF); }
void target_off(int pwr) { COMMAND(EXECUTE_SCRIPT, 2, VDD_OFF, pwr ? VDD_GND_ON : VDD_GND_OFF); }
void target_on(int pwr)  { COMMAND(EXECUTE_SCRIPT, 2, VDD_GND_OFF, pwr ? VDD_ON : VDD_OFF); usleep(50000); }
void uart_off(void)      { COMMAND(EXIT_UART_MODE); }
void uart_on(float rate) {
    int irate = (65536.0f - (((1.0f / rate) - .000003f) / .000000167f));
    COMMAND(CLR_UPLOAD_BFR);
    COMMAND(CLR_DOWNLOAD_BFR);
    COMMAND(ENTER_UART_MODE, irate & 0xFF, (irate >> 8) & 0xFF);
}

void dots(int n) { while (n--) LOG("."); }


/* Transfer data between fds and the PK2.  These use the lower level
   transfer functions. */
int transfer_input(int fd, int max_tx) {
    char buf[BUFFER_SIZE];
    memset(buf, END_OF_BUFFER, BUFFER_SIZE);

    buf[0] = DOWNLOAD_DATA;    
    buf[1] = read(fd, buf+2, max_tx);

    if (-1 == buf[1]) return buf[1];

    LOG("I %2d\n", buf[1]);
    usb_send_buf(buf);
    return buf[1];
}

void transfer_output(int fd) {   // from PK2 -> host
    char buf[BUFFER_SIZE];
    char cmd[] = {UPLOAD_DATA};
    int written;

    usb_send(cmd, 1);
    usb_receive_buf(buf);

        LOG("      O %2d\n", buf[0]);

    if (buf[0]) { 

        written = write(fd, buf + 1, buf[0]); 
        if (written != buf[0]) {
            RAISE_ERROR("output buffer overrun!\n");
        }
    }
}

/* Signal handler */
int stop = 0;
void handler(int signal) { LOG("got signal %d\n", signal); stop = 1; }


/* Event loop */
void start_serial(void) {

    int eof = 0;

    /* Compute optimal buffer size depending on desired latency. */
    int usec_per_byte = (10 * (1000000 / baud_rate));

    LOG("latency = %d bytes\n", latency_bytes);


    /* Setup PK2 with target off.  FIXME: connect to running target! */
    reset_hold(reset_target);
    target_off(power_target);
    if (power_target) set_vdd(5.0);

    /* Start target. */
    target_on(power_target);
    reset_release();

    /* Check if votage is ok. */
    LOG("status %04X\n", get_status());

    /* Go */
    if (-1 == fcntl(input_fd, F_SETFL, O_NONBLOCK))  { 
        RAISE_ERROR("Can't set input_fd O_NONBLOCK.\n"); 
    }
    uart_on(baud_rate);
    while(!stop) {

        if (eof) {
            /* If input is closed we cn just poll the output. */
            LOG("EOF\n");
            usleep(usec_per_byte);
            transfer_output(output_fd);
        }
        else {
            /* When there's room in the buffer, always respond immediately
               to input data. */
            struct timeval timeout;
            fd_set inset;

            timeout.tv_sec = 0;
            timeout.tv_usec = usec_per_byte * latency_bytes;
            FD_ZERO(&inset);
            FD_SET(input_fd, &inset);
            if (-1 == (select(1+input_fd, &inset, NULL, NULL, &timeout))) {
                if (errno != EINTR) {
                    RAISE_ERROR("select() error\n");
                }
            }

            /* If there's data, read as much as possible. */
            if (FD_ISSET(input_fd, &inset)) {
                int rv;
                rv = transfer_input(input_fd, latency_bytes);
                if (rv == -1) { 
                    if (errno == EAGAIN) break;
                    RAISE_ERROR("read(): %s\n",strerror(errno)); 
                }
                if (rv == 0) {
                    eof = 1;
                    if (exit_on_eof) break;
                }
                
                /* Simple rate limiting mechanism: per transfer, wait for
                   as long as it takes to transfer the bytes.  Note that
                   this can be made more efficient when we track an
                   estimate of the number of elements in the upload
                   buffer, keeping the buffer full instead of empty. */
                
                while(rv--) {
                    if (stop) break;
                    usleep(usec_per_byte);
                }
            }
        }
        transfer_output(output_fd);
    }

    /* Cleanup */
    uart_off();
    reset_pk2();
}
void usage(void){
    fprintf(stderr,
            "usage: pk2serial [<option> ...]\n"
            "  options:\n"
            "    -b rate         Transmission rate (9600 baud)\n"
            "    -l bufsize      Latency (32 bytes)\n"
            "    -e              Exit on EOF\n"
            "    -r              Reset target\n"
            "    -p              Power target (Spec says not to!)\n"
            "    -v              Verbose output\n"
            "    -h              This help text.\n"

);
}

void usage_exit(void){
    usage();
    exit(1);
}

char limit_fmt[] = "limited %s to %f";

#define LIMIT(variable, op, value) { if (variable op value) variable = value; }

int main(int argc, char **argv) {
    struct usb_bus *busses;

    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "rpevhb:l:")) != -1) {
        switch(c) {
        case 'h': usage(); exit(0);
        case 'p': power_target = 1; break;
        case 'r': reset_target = 1; break;
        case 'v': verbose++; break;
        case 'e': exit_on_eof = 1; break;
        case 'b': 
            baud_rate = strtol(optarg, NULL, 10);
            LIMIT(baud_rate, <, 10);     // arbitrary limit at 1 byte / second.
            LIMIT(baud_rate, >, 60000);  // tested with loopback
            break;

        case 'l': 
            latency_bytes = strtol(optarg, NULL, 10);
            LIMIT(latency_bytes, <, 1);
            LIMIT(latency_bytes, >, 60);
            break;
        case '?':
            if ((optopt == 'b') || (optopt == 'l'))
                fprintf (stderr, "Option -%c requires an argument.\n", 
                         optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
        default:
            usage_exit();
        }
    }

    LOG("baud rate = %d\n", baud_rate);

    
    signal(SIGINT, handler);

    usb_init();
    usb_find_busses();
    usb_find_devices();
    
    busses = usb_get_busses();

    struct usb_bus *bus;
    
    for (bus = busses; bus; bus = bus->next) {
        struct usb_device *dev;
        
        for (dev = bus->devices; dev; dev = dev->next) {

            /* Find first PK2 */
            if ((dev->descriptor.idVendor  == vendor)
                && (dev->descriptor.idProduct == product)) {

                if (!(handle = usb_open(dev))) {
                    RAISE_ERROR("Can't open device.\n");
                }
                /* FIXME: Detach if necessary 
                   usb_get_driver_np
                   usb_detach_kernel_driver_np
                 */
                if (-1 == usb_set_configuration(handle, 2)) { // Try vendor config (not HID!)
                    RAISE_ERROR("Can't set vendor config.\n");
                }
                if (-1 == usb_claim_interface(handle, 0)) {
                    RAISE_ERROR("Can't claim interface.\n");
                }
                start_serial();
                if (-1 == usb_release_interface(handle, 0)) {
                    RAISE_ERROR("Can't release interface.\n");
                }
                if (-1 == usb_close(handle)) {
                    RAISE_ERROR("Can't close device.\n");
                }
            }
        }
    }
    return 0;
    
}
