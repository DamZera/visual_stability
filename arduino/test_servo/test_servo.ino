#include <Servo.h>

Servo servYaw;
Servo servPitch;
char buff[40];
int yaw = -1;
int pitch = -1;
int i = 0;

char car;

void setup() {
  Serial.begin(9600);
  //servYaw.attach(3);
  //servPitch.attach(5);
}

void readSerialToServos(){
  while(Serial.available() > 0){
    car = Serial.read();
    if(car != '\n'){
      buff[i] = car;
      buff[i+1] = '\0';
      i++;
    } else{
      sscanf(buff, "Y:%d:P:%d", &yaw, &pitch);
      //Serial.print("'");
      //Serial.print(buff);
      //Serial.println("'");
      i=0;
    }
  }
}

void loop() {
  readSerialToServos();
  if(yaw != -1 || pitch != -1){
    Serial.print("Yaw:");
    Serial.print(yaw);
    Serial.print("Pitch:");
    Serial.println(pitch);
    servYaw.write(yaw);
    servPitch.write(pitch);
  }
  yaw=-1;pitch=-1;
}
