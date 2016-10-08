#include "serial.h"
//#define SERIALPORTDEBUG

struct termios toptions;

int serial_open(serial_com *sp, char* name){
	strcpy(sp->nom_port, name);
	//int fd = open(sp->nom_port, O_RDWR | O_NOCTTY | O_NDELAY);
	int fd = open(sp->nom_port, O_RDWR | O_NONBLOCK );
	sp->fd = fd;
	if(fd == -1){
		perror("open :: ");
	}
	usleep(300000);
	tcgetattr(sp->fd, &toptions);

	/*---- CONFIGURATION 9600 8N1 ----*/
	cfsetispeed(&toptions, (speed_t)B9600);
	cfsetospeed(&toptions, (speed_t)B9600);
	toptions.c_cflag &= ~PARENB;
	toptions.c_cflag &= ~CSTOPB;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8;

	toptions.c_cflag &= ~CRTSCTS;

	toptions.c_cflag |= CLOCAL | CREAD;

    toptions.c_iflag |= IGNPAR | IGNCR;
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    toptions.c_oflag &= ~OPOST;

    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;
	
	tcsetattr(sp->fd, TCSANOW, &toptions);
	if( tcsetattr(sp->fd, TCSAFLUSH, &toptions) <0){
		perror("init_serialport: Couldn't set term attribues");
	return -1;
	}

	return fd;
}

void serial_close(serial_com* sp){
 	close(sp->fd);
}

int serial_write(serial_com* sp, char* buff){
	int len = strlen(buff);
	int test = write(sp->fd, buff, len);
	if(test != len)
		printf("serialport_write: couldn't write whole string\n");
	return test;
}

int serial_read(serial_com* sp, char* buf, char until,  int buf_max, int timeout){
    char b[1];
    int i=0;

    do { 
        int n = read(sp->fd, b, 1);

        if( n==-1) return -1;
        if( n==0 ) {
            usleep( 1 * 1000 );
            timeout--;
            if( timeout==0 ) return -2;
            continue;
        }

#ifdef SERIALPORTDEBUG  
		printf("serialport_read_until: i=%d, n=%d b='%c'\n",i,n,b[0]);
#endif
        buf[i] = b[0]; 
        i++;
    } while( b[0] != until && i < buf_max && timeout>0 );

    buf[i] = 0;
    return 0;
}

int servAngle(serial_com *sp, int angleY, int angleP){
	char *buffer = new char[20];
	int test;

	sprintf(buffer, "Y:%d:P:%d+", angleY, angleP);
	test = serial_write(sp, buffer);
#ifdef SERIALPORTDEBUG  
		printf("Write: %d  // %s\n", test, buffer);
#endif
	return test;
}

