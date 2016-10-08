#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct serial_com{
	char nom_port[100];
	int fd;
}serial_com;

int serial_open(serial_com *sp, char *name);
void serial_close(serial_com* sp);
int serial_write(serial_com* sp, char* buff);
int serial_read(serial_com* sp, char* buf, char until,  int buf_max, int timeout);
int servAngle(serial_com *sp, int angleY, int angleP);