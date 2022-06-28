float calibration_value = 213.50 ;//215.43 212.28 213.13 215.43 213.35

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
movingAvg avgThrust(10); 
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
      Data.println("thrust(N)");
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
  force_value = (force())* 100000;
  force_value_avg = avgThrust.reading(force_value);
  force_value_avg_fixed = (force_value_avg/100000);
  saveData(force_value_avg_fixed, tijd);
  Serial.println(force_value_avg_fixed,5);
  Serial.println(tijd);

}

float force(){
  weight = scale.get_units();
  gravity = (weight / 1000) * 9.81;
  return gravity;
}

void saveData(float x, int t){
  Data = SD.open("Data.csv", FILE_WRITE);
  if(Data){
    Data.print(t);
    Data.print(",");
    Data.println(x,5);
    }
   Data.close();
  }
