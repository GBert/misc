/* src/config.h.  Generated from config.h.in by configure.  */
/* src/config.h.in.  Generated from configure.ac by autoheader.  */

/* Activate IP V6 */
#define ENABLE_IPV6 1

/* Define to 1 if you have the <dev/ppbus/ppi.h> header file. */
/* #undef HAVE_DEV_PPBUS_PPI_H */

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* define to 1 if you have the getaddrinfo function */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `ioperm' library (-lioperm). */
/* #undef HAVE_LIBIOPERM */

/* Define to 1 if you have the <linux/i2c-dev.h> header file. */
#define HAVE_LINUX_I2C_DEV_H 1

/* Define to 1 if you have the <linux/serial.h> header file. */
#define HAVE_LINUX_SERIAL_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD */

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H 1

/* Define to 1 if you have the <sys/fileo.h> header file. */
/* #undef HAVE_SYS_FILEO_H */

/* Define to 1 if you have the <sys/io.h> header file. */
#define HAVE_SYS_IO_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* define this to include own cfmakeraw() */
/* #undef NO_CFMAKERAW */

/* Name of package */
#define PACKAGE "srcpd"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "srcpd-devel@lists.sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "srcpd"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "srcpd 2.1.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "srcpd"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.1.3"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Raspberry PI found */
#define BANANAPI 1
/* #undef RASPBERRYPI - für BPI geändert RM */
#define RASPBERRYPI 1
#define BANANAPI_BOARD 1
/* Raspberry PI 1 Board Version 1 found */
/* #undef RASPBERRYPI_BOARD_V1 */

/* Raspberry PI 1 Board Version 2 found */
/* #undef RASPBERRYPI_BOARD_V2 */

/* Raspberry PI Model 2 or 3 found */
/* #undef RASPBERRYPI_MODEL_2_3 */

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* define to 1 if you want canbus support */
//#define USE_CANBUS 1

/* define to 1 if you want ddl support */
#define USE_DDL 1

/* define to 1 if you want ddl-s88 support */
//#define USE_DDLS88 1

/* define to 1 if you want hsi-88 support */
//#define USE_HSI88 1

/* define to 1 if you want i2c-dev support */
//#define USE_I2C 1

/* define to 1 if you want intellibox support */
//#define USE_IB 1

/* define to 1 if you want li100 (RS232 + USB) support */
//#define USE_LI100 1

/* define to 1 if you want loconet support */
//#define USE_LOCONET 1

/* define to 1 if you want loopback support */
#define USE_LOOPBACK 1

/* define to 1 if you want m605x support */
//#define USE_M605X 1

/* define to 1 if you want selectrix support */
//#define USE_SELECTRIX 1

/* define to 1 if you want zimo support */
//#define USE_ZIMO 1

/* Version number of package */
#define VERSION "2.1.3"

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */
