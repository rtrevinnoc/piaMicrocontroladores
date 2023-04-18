/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;
String posData = "";

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void moveServo(uint8_t n_servo, int angulo) {
  int duty;
  duty=map(angulo,0,180,SERVOMIN,SERVOMAX);
  pwm.setPWM(n_servo, 0, duty);  
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("8 channel Servo test!");

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}

void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (inChar) {
      posData += (char)inChar;
    }
    if (inChar == '\n') {
      posData.trim();
//      Serial.println(posData);

      String xValueString = getValue(posData, ',', 0);
      int xValue = xValueString.toInt();
      
      String yValueString = getValue(posData, ',', 1);
      int yValue = yValueString.toInt();
      
      String doorOneValueString = getValue(posData, ',', 2);
      int doorOneValue = doorOneValueString.toInt();
      
      String doorTwoValueString = getValue(posData, ',', 3);
      int doorTwoValue = doorTwoValueString.toInt();

      Serial.print("X:" + xValueString);
      Serial.print(";Y:" + yValueString);
      Serial.print(";Door1:" + doorOneValueString);
      Serial.println(";Door2:" + doorTwoValueString);

      moveServo(0,xValue);
      moveServo(1,yValue);
      moveServo(2,doorOneValue);
      moveServo(3,doorTwoValue);
      
      posData = "";
    }
  }

  
//  while (!Serial.available());
//      String posData = Serial.readString();
//      delay(600);
//      Serial.println("Python: " + posData);
//       
//      String xValueString = getValue(posData, ',', 0);
//      int xValue = xValueString.toInt();
//      
//      String yValueString = getValue(posData, ',', 1);
//      int yValue = yValueString.toInt();
//      
//      String doorOneValueString = getValue(posData, ',', 2);
//      int doorOneValue = doorOneValueString.toInt();
//      
//      String doorTwoValueString = getValue(posData, ',', 3);
//      int doorTwoValue = doorTwoValueString.toInt();
//  
//      Serial.println("X:" + xValueString);
//      Serial.print(yValue);
//      Serial.print(doorOneValue);
//      Serial.print(doorTwoValue);
//      Serial.println();
//       
//      moveServo(0,xValue);
//      moveServo(1,yValue);
//      moveServo(2,doorOneValue);
//      moveServo(3,doorTwoValue);
}
