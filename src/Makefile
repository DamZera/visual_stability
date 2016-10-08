all: color_tracking

color_tracking: color_tracking.o serial.o
	g++ color_tracking.o serial.o -o color_tracking -lopencv_core -lopencv_highgui -lopencv_imgproc

color_tracking.o: serial.C color_tracking.C
	g++ color_tracking.C serial.C -c 

serial.o: serial.C
	g++ serial.C -c

clean:
	rm *.o

mrproper:
	rm color_tracking
	rm *.o
