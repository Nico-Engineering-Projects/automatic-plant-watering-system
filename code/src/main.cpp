#include <Arduino.h>
#include 
#define pumpPin 14 //Defining the relay pin for future use

// put function declarations here:


void setup() {
  // put your setup code here, to run once:
 pinMode(pumpPin, OUTPUT);//Setting the relay pin as output
}

void loop() {
 digitalWrite(pumpPin, HIGH); //Turning the relay off
 delay(5000); //Waiting for 10 Seconds
 digitalWrite(pumpPin, LOW); //Turning the relay on
 delay(10000); //Waiting for 20 Seconds
}

