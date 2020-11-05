#include "MPU9250.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
SoftwareSerial s(5,6);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int status;
const int buzzer = 3, button = 4;
float previousTime, currentTime, elapsedTime;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
float gyroAngleX_deg, gyroAngleY_deg;
float gyroX, gyroY,gyroZ,accX,accY,accZ;
float roll, pitch;
bool fallen = false;


void setup() {
  s.begin(115200);
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}
  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4,0);

  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  
  // read the sensor
  IMU.readSensor();
  gyroX = IMU.getGyroX_rads();
  gyroY = IMU.getGyroY_rads();
  gyroZ = IMU.getGyroZ_rads();
  accX = IMU.getAccelX_mss();
  accY = IMU.getAccelY_mss();
  accZ = IMU.getAccelZ_mss();

  accAngleX = (atan(accY / sqrt(pow(accX, 2) + pow(accZ, 2))) * 180 / PI);
  accAngleY = (atan(-1 * accX / sqrt(pow(accY, 2) + pow(accZ, 2))) * 180 / PI);

  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime-previousTime)/1000;
  
  gyroAngleX = gyroAngleX + gyroX * elapsedTime;
  gyroAngleY = gyroAngleY + gyroY * elapsedTime;

  gyroAngleX_deg = (gyroAngleX * 180) / PI;
  gyroAngleY_deg = (gyroAngleY * 180) / PI;
  
  roll = 0.955 * gyroAngleX_deg + 0.045 * accAngleX;
  pitch = 0.955 * gyroAngleY_deg + 0.045 * accAngleY;

  int snooze;
  snooze = digitalRead(button);

  Serial.print("roll and pitch : ");
  Serial.print(roll);
  Serial.print("\t");
  Serial.println(pitch);

//  if (snooze == LOW) {
//    Serial.println("The button is being pressed");
//    digitalWrite(buzzer, HIGH); // turn on
//  }
//  else
//  if (snooze == HIGH) {
//    Serial.println("The button is unpressed");
//    digitalWrite(buzzer, LOW);  // turn off
//  }

  if(roll > 50.0 || pitch > 50.0){
    //Serial.println("Loop Entered");
    //fallen = true;
	lcd.print("FALL DETECTED!");
    while(snooze == HIGH){
      digitalWrite(buzzer, HIGH);
	  s.write(1);
      Serial.println("Buzzer High");
      snooze = digitalRead(button);
	  delay(5);
    }
  }
//  else{
    digitalWrite(buzzer, LOW);
	lcd.print("Wheel chair is steady.");
//    s.write(0);
//  }
  delay(100);
}