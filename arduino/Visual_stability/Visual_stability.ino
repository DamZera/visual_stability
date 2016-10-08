#include <Servo.h>

Servo servYaw;
Servo servPitch;
char buff[40];
int yaw;
int pitch;
int i = 0;

char car;

void setup() {
  Serial.begin(9600);
  servYaw.attach(5);
  servPitch.attach(3);
  pitch=60;yaw=90;
}

void readSerialToServos(){
  while(Serial.available() > 0){
    car = Serial.read();
    
    if(car != '+'){
      buff[i] = car;
      buff[i+1] = '\0';
      i++;
    } else{
      sscanf(buff, "Y:%d:P:%d", &yaw, &pitch);
      i=0;
    }
  }
}

void loop() {
  readSerialToServos();
    
    servYaw.write(yaw);
    servPitch.write(pitch);
}
