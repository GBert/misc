// config.h - adapted for basrcpd project 2018 by Rainer Müller 

/* Activate IP V6 */
#define ENABLE_IPV6 1

/* which PI is used */
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


/* define to 1 if you want canbus support */
#define USE_CANBUS 1

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
#define VERSION "2102"
