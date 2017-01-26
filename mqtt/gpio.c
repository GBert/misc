#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define IN	0
#define OUT	1
 
#define LOW	0
#define HIGH	1

#define MAX_BUFFER      256

int gpio_export(int pin) {
	char buffer[3];
	ssize_t bytes_written;
	int fd, ret;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(EXIT_FAILURE);
	}
 
	bytes_written = snprintf(buffer, 3, "%d", pin);
	ret = write(fd, buffer, bytes_written);
	if (ret < 0) {
		fprintf(stderr, "Failed to export GPIO %d\n", pin);
		return(EXIT_FAILURE);
	}
	close(fd);
	return(0);
}

int gpio_unexport(int pin) {
	char buffer[3];
	ssize_t bytes_written;
	int fd, ret;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(EXIT_FAILURE);
	}
 
	bytes_written = snprintf(buffer, 3, "%d", pin);
	ret = write(fd, buffer, bytes_written);
	if (ret < 0) {
		fprintf(stderr, "Failed to unexport GPIO %d\n", pin);
		return(EXIT_FAILURE);
	}
	close(fd);
	return(0);
}

int gpio_direction(int pin, int dir) {
	static const char s_directions_str[]  = "in\0out";
 
#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(EXIT_FAILURE);
	}
 
	if (write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3) < 0) {
		fprintf(stderr, "Failed to set direction!\n");
		return(EXIT_FAILURE);
	}
 
	close(fd);
	return(0);
}

int gpio_read(int pin) {
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(EXIT_FAILURE);
	}
 
	if (read(fd, value_str, 3) < 0) {
		fprintf(stderr, "Failed to read value!\n");
		return(EXIT_FAILURE);
	}
 
	close(fd);
 
	return(atoi(value_str));
}

int gpio_write(int pin, int value) {
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
 
	if (write(fd, &s_values_str[LOW == value ? 0 : 1], 1) != 1) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
