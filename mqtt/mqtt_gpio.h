#ifndef _MQTT_GPIO_H_
#define _MQTT_GPIO_H_

#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <mosquitto.h>

#define mqtt_host "localhost"
#define mqtt_port 1883

#define IN	0
#define OUT	1

#define LOW	0
#define HIGH	1

#define MAX_BUFFER      256

int gpio_export(int pin);
int gpio_unexport(int pin);
int gpio_direction(int pin, int dir);
int gpio_read(int pin);
int gpio_write(int pin, int value);

#endif /* _MQTT_GPIO_H_ */
