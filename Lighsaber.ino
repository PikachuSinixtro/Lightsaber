#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <I2Cdev.h>

#define NUMPIXELS 60

#define PIN 2

MPU6050 mpu6050(Wire);

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int RX = 3;
int TX = 4;

SoftwareSerial softserial(RX, TX);

DFRobotDFPlayerMini DFP;

boolean on = false;
boolean playing = false;

int ledcolor = 1;

void setup(){

  pixels.begin();

  pixels.clear();

  Wire.begin();
  mpu6050.begin();
  delay(100);

  mpu6050.calcGyroOffsets(true);  

  pinMode(8, INPUT);

  Serial.begin(9600);
  softserial.begin(9600);
  
  if(DFP.begin(softserial)){
    Serial.println("Player Online");
    DFP.play(9);
  }else{
    Serial.println("Unable to begin");
  }

  DFP.volume(30);


}

void loop(){ 

 

  mpu6050.update();
  
  int16_t accX = mpu6050.getAccX();
  int16_t accY = mpu6050.getAccY();
  int16_t accZ = mpu6050.getAccZ();

  float gyroY = mpu6050.getGyroY();

  float magnitude = sqrt(pow(accX, 2) + pow(accY, 2) + pow(accZ, 2));

  float pitch = atan2(-accY, accZ) * 180 / M_PI;

  Serial.println(gyroY);

  if(pitch == 90 && !on){
    if(abs(gyroY) > 400){

      ledcolor++;

      if(ledcolor == 4){
        ledcolor = 1;
      }  

      if(ledcolor == 1){
        DFP.play(9);
      }else if(ledcolor == 2){
        DFP.play(10);
      }else if(ledcolor == 3){
        DFP.play(11);
      }

      delay(1000);
    }
    
  }

 if (!on && digitalRead(8) == HIGH){

  DFP.play(3);
  on = true;
  playing = false; 
  DFP.enableLoop();
  delay(500);

  switch(ledcolor){
    case 1:
      ledson(0, 255, 0);
      break;

    case 2:
      ledson(255, 0, 0);
      break;

    case 3:
      ledson(0, 0, 255);
      break;
  }


 }



 /*****************************************************************/


 if(on){

  if (on && !playing){

    DFP.loop(1);
    playing = true;
    delay(100);
    
  }

  if(magnitude >= 1.5 && playing){
      
    DFP.play(random(4, 8));
    playing = false; 
    delay(700);  

  } 

 }

 /**********************************************************************/

 if (on && digitalRead(8) == HIGH){
 
  DFP.play(2);
  playing = false;
  on = false;
  delay(500);
  DFP.disableLoop();
  ledsoff();
  
 }

  
}

void ledson(int green, int red, int blue){

  for(int i=0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(green, red, blue));
    pixels.setPixelColor(i+1, pixels.Color(green, red, blue));
    pixels.setPixelColor(i+2, pixels.Color(green, red, blue));
    pixels.show();
    delay(10);
  }


}

void ledsoff(){

  for(int i=60; i >= 0; i--){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(i-2, pixels.Color(0, 0, 0));
    pixels.show();
    delay(10);
  }

}
