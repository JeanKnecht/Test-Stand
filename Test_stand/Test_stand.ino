float calibration_value = 215.70 ;//215.10 215.27 215.026 215.17

#include <Arduino.h>
#include "HX711.h"
#include <SD.h>
#include <SPI.h>

// HX711 circuit wiring
HX711 scale;
const int LOADCELL_DOUT_PIN = 4; //pins for making connecetion with the amplifier
const int LOADCELL_SCK_PIN = 5;
float weight;
float gravity;
float force_value;
int tijd;

//SD logger
File Data;
int pinCS = 10;
bool SD_actief = true;

//LED and Button

int led = 3;
int button = 2;

void setup() {
  //delay(10000);
  Serial.begin(9600);
  
  pinMode(pinCS, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(led, HIGH);
  
  while(SD_actief){
      SD.begin();
      Data = SD.open("Data.csv", FILE_WRITE);
      if(Data){
        //Serial.println("sd is actief");
        Data.print("time(s)");
        Data.print(",");
        Data.println("thrust(N)");
        while(SD_actief){
          led_SD();
          int trigger = digitalRead(button);
          if(trigger == 1){
            SD_actief = false;
            led_armed();
            }
          }
        }
        
      else{
        //Serial.println("nog niet actief");
        }       
      delay(1000);
   }


  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);    
  scale.set_scale(calibration_value);
  scale.tare();
}

void loop() {
  tijd = millis();
  force_value = force();
  saveData(tijd, force_value);
  
  int trigger = digitalRead(button);
  if(trigger == 1){
    Data.close();
    led_armed();
    exit(0);
    }

}

float force(){
  weight = scale.get_units();
  gravity = (weight / 1000) * 9.81;
  return gravity;
}

void saveData(int t, float x){
  Data.print(t);
  Data.print(",");
  Data.println(x,5);
  }
void led_SD(){
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  }
void led_armed(){
  for(int i = 0; i<8; i++){
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    delay(100);
    }
  }
