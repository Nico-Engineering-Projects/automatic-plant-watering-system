#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define pumpPin 14 //Defining the relay pin for future use
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
int temp = 0;
int humidity = 0;
int soilMoisture = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// put function declarations here:


void setup() {
  // put your setup code here, to run once:
 pinMode(pumpPin, OUTPUT);//Setting the relay pin as output
 digitalWrite(pumpPin, HIGH); //Turning the relay off
 //Setting up the OLED display
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 0);
display.println("Welcome");
display.display();
delay(3000); //Waiting for 2 Seconds

}

void data()
{
display.setCursor(0,0);
display.println("Soil Moisture: " + String(soilMoisture) + "%");
display.setCursor(0,10);
display.println("Temperature: " + String(temp) + "C");
display.setCursor(0,20);
display.println("Humidity: " + String(humidity) + "%");
display.display();
}

void loop() {
display.setTextSize(1);
digitalWrite(pumpPin, HIGH); //Turning the relay off
display.clearDisplay();
data();
display.setCursor(0,30);
display.println("Pump is OFF");
display.display();
delay(5000); //Waiting for 10 Seconds
digitalWrite(pumpPin, LOW); //Turning the relay ON
display.clearDisplay();
display.display();
data();
display.setCursor(0,30);
display.println("Pump is ON");
display.display();
delay(5000); //Waiting for 10 Seconds


}



