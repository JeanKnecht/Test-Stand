float calibration_value = 215.14 ;//215.10 215.27 215.026 215.17

#include <Arduino.h>
#include "HX711.h"
#include <SD.h>
#include <SPI.h>
#include <movingAvg.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4; //pins for making connecetion with the amplifier
const int LOADCELL_SCK_PIN = 5;
float weight;
float gravity;
float force_value;
int tijd;

//SD logger
File Data;
int pinCS = 10;

//moving average filter
movingAvg avgThrust(8); 
float force_value_avg;
float force_value_avg_fixed;
HX711 scale;

void setup() {
  //delay(10000);
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  if (SD.begin()){
    Data = SD.open("Data.csv", FILE_WRITE);
    if(Data){
      Data.print("time(s)");
      Data.print(",");
      Data.print("thrust(N)");
      Data.print(",");
      Data.println("thrust RAW(N)");
      Data.close();
      }
  
    }else{
      return;
      }
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);    
        
  scale.set_scale(calibration_value);
  scale.tare();

  avgThrust.begin();
}

void loop() {
  tijd = millis();
  force_value = force();
  force_value_avg = avgThrust.reading(force_value*100000);
  force_value_avg_fixed = (force_value_avg/100000);
  saveData(force_value_avg_fixed, tijd, force_value);
  Serial.println(force_value_avg_fixed,5);
  Serial.println(tijd);

}

float force(){
  weight = scale.get_units();
  gravity = (weight / 1000) * 9.81;
  return gravity;
}

void saveData(float x, int t, float y){
  Data = SD.open("Data.csv", FILE_WRITE);
  if(Data){
    Data.print(t);
    Data.print(",");
    Data.print(x,5);
    Data.print(",");
    Data.println(y,5);
    }
   Data.close();
  }
