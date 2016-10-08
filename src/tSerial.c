#include "serial.h"

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

int main(void){
	serial_com sp;
	char *buffer = new char[80];
	int test;

	serial_open(&sp, "/dev/cu.usbmodem1411");

	printf("Fic : %d\n", sp.fd);

	sleep(1);
	for(int i=45; i<120; i++){
		servAngle(&sp, i, i);
		usleep(15000);
	}
	serial_close(&sp);
}