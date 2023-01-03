#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <MPU6050.h>

int16_t ax, ay, az;
int16_t gx, gy, gz;

float vectorprevious;
float vector;
float totalvector;

int steps = 0;
char axs[6];
char ays[6];
char azs[6];

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

MPU6050 accelgyro;

void setup() {
  Serial.begin(9600);
  Serial.println("LED matrix step counter test.");

  Wire.begin(); // For I2C and SPI  

  matrix.begin(0x70);  // pass in the address

  delay(100);
  
  matrix.clear();
  matrix.fillRoundRect(1, 1, 6, 6, 2, LED_ON);
  matrix.fillRoundRect(3, 3, 2, 2, 1, LED_OFF);
  delay(1000);
  matrix.writeDisplay();

  Serial.println("Gryo..");
  accelgyro.initialize();
  accelgyro.CalibrateAccel();
  Serial.println("OK");
  
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  //matrix.fillCircle(0, 0, 3, 0);
  //delay(1000);
  //matrix.clear();
}

void get_movement(){
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  /*Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
  Serial.println();*/
}

void display_steps(){
  matrix.clear();
  uint8_t total = 0;
  for(uint8_t i = 0; i < 8; i++){
    for(uint8_t j = 0; j < 8; j++){
      total++;
      if(total < steps){
        matrix.drawPixel(i, j, LED_ON);
      }
    }
  }
  matrix.writeDisplay(); 
}

void loop() {
  get_movement();
  vector = sqrt( (ax * ax) + (ay * ay) + (az * az) );
  totalvector = vector - vectorprevious;
  Serial.println(totalvector);
  if(totalvector > 600){
    
    Serial.print("Step! ");
    Serial.println(steps);
    steps++;
    if(steps > 64){
      steps = 0;
    }
  }
  display_steps();
  delay(200);
  vectorprevious = vector;
}