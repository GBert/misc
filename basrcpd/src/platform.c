// platform.c : PI platform access 
//
// C 2018 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#include <stdio.h>
#include <string.h>
#include "platform.h"

#define BUF 255

// valid at least for BananaPi running with Armbian, CentOS, and Lede
// and for RaspberryPi with Raspbian
#define PLSERIALFILE "/proc/cpuinfo" 
#define PLSERIALKEY  "Serial"
#define PLTEMPERFILE "/sys/class/thermal/thermal_zone0/temp"

static uint64_t serial = 0;
static FILE *temperfile = NULL;

uint64_t getPlatformData(pl_function_t fucode)
{
   	char *found, buffer[BUF];
   	long int temper = 0;

	switch(fucode) {
		case PL_SERIAL:	if (serial == 0) {
							FILE *sfile = fopen(PLSERIALFILE, "r");
   							if(sfile == NULL) break;
   							while( fgets(buffer, BUF, sfile) != NULL ) {
								found = strstr(buffer, PLSERIALKEY);
								if (found) sscanf(found+strlen(PLSERIALKEY),
														" : %16llx", &serial);
							}
							fclose(sfile);
   						}
   						return serial;
   						
		case PL_TEMP:   if (temperfile == NULL) {
							temperfile = fopen(PLTEMPERFILE, "r");
   							if(temperfile == NULL) break;
						}
						else rewind(temperfile);
						fread(buffer, 8, 1, temperfile);
                        sscanf(buffer, "%ld", &temper);
#ifdef REOPEN
                        fclose(temperfile);
                        temperfile = NULL;
#endif
						return temper;
						
		default:        break;
	}
	printf("Fehler aufgetreten... \n");  	// TODO: => syslog_bus ...
	return 0L;
}

// -----------------------------------------------------------------------------
#ifdef TEST
// noch notwendige Header
#include <unistd.h>
#include <sys/time.h>

// Variablendeklaration
time_t t_now = 0;
struct timeval tv = { 0, 0 };

uint64_t getPlatformTime(pl_function_t fucode)
{
	// Start Zeitmessung
	gettimeofday(&tv, NULL);
	t_now = tv.tv_sec * 1000000 + tv.tv_usec;

	uint64_t retval = getPlatformData(fucode);

	// Ende Zeitmessung und Ausgabe
	gettimeofday(&tv, NULL);
	t_now = tv.tv_sec * 1000000 + tv.tv_usec - t_now;
	printf("Ausführungsdauer %dµs für Abfrage:\n", t_now);
	return retval;
}

int main(int argc, char *argv[]) 
{
   	uint64_t serial;
	for (int i=0; i<10; i++) {
		if ((i & 7) == 0) {
			serial = getPlatformTime(PL_SERIAL);
			printf("Serial : %llx\n", serial);
			serial %= 1000000L;
			printf("Serial : %lld\n", serial);
		}
		printf("Temperatur : %lld\n", getPlatformTime(PL_TEMP)/100);
		sleep(2);
    }
   	return 0;
}
#endif
