#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


// =====================================================
// PINS
// =====================================================

#define pumpPin 13
#define tempSensorPin 15
#define moistureSensorPin 2


// =====================================================
// OLED
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


// =====================================================
// DHT
// =====================================================

#define DHTTYPE DHT11


// =====================================================
// SOIL CALIBRATION
// =====================================================

#define drySoil 3155
#define wetSoil 1540


// =====================================================
// WATERING SETTINGS
// =====================================================

#define DRY_LEVEL 5
#define WET_LEVEL 80


// =====================================================
// TIME CONSTANTS
// =====================================================

// Pump runs for 5 seconds
const unsigned long pumpRunTime = 5000UL;

// Allow water to soak for 5 minutes
const unsigned long soakTime = 300000UL;

// Read sensors every 2 seconds
const unsigned long checkInterval = 2000UL;

// 3 days in milliseconds
const unsigned long threedays =
    3UL * 24UL * 60UL * 60UL * 1000UL;


// =====================================================
// SENSOR VARIABLES
// =====================================================

float temp = 0;
float humidity = 0;

int soilMoisture = 0;
int rawSoilMoisture = 0;


// =====================================================
// BOOLEAN VARIABLES
// =====================================================

// Has the plant ever successfully reached 80%?
bool Watered = false;

// Is the system currently performing a watering cycle?
bool wateringMode = false;

// Is the pump physically running?
bool pumpRunning = false;


// =====================================================
// TIMER VARIABLES
// =====================================================

unsigned long pumpStartTime = 0;
unsigned long soakStartTime = 0;
unsigned long lastSensorRead = 0;

unsigned long lastWateredTime = 0;
unsigned long systemStartTime = 0;


// =====================================================
// OBJECTS
// =====================================================

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,-1);

DHT dht(tempSensorPin, DHTTYPE);


// =====================================================
// READ SENSORS
// =====================================================

void readSensors()
{
    // Read raw soil moisture
    rawSoilMoisture = analogRead(moistureSensorPin);

    // Convert raw reading to percentage
    soilMoisture = map(rawSoilMoisture,drySoil,wetSoil,0,100);

    // Keep moisture percentage between 0 and 100
    soilMoisture = constrain(soilMoisture,0,100);


    // Read DHT11
    temp = dht.readTemperature();
    humidity = dht.readHumidity();


    // Serial debugging
    Serial.print("Raw Soil: ");
    Serial.print(rawSoilMoisture);

    Serial.print(" | Moisture: ");
    Serial.print(soilMoisture);
    Serial.print("%");

    Serial.print(" | Temp: ");
    Serial.print(temp);

    Serial.print(" C");

    Serial.print(" | Humidity: ");
    Serial.print(humidity);

    Serial.println("%");
}


// =====================================================
// NORMAL OLED DISPLAY
// =====================================================

void displayData(const char *message)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);


    // Soil moisture
    display.setCursor(0, 0);

    display.print("Soil: ");
    display.print(soilMoisture);
    display.println("%");


    // Temperature
    display.setCursor(0, 10);

    display.print("Temp: ");
    display.print(temp, 1);
    display.println(" C");


    // Humidity
    display.setCursor(0, 20);

    display.print("Humidity: ");
    display.print(humidity, 0);
    display.println("%");


    // System status
    display.setCursor(0, 35);
    display.println(message);


    // Pump status
    display.setCursor(0, 48);

    if (pumpRunning)
    {
        display.println("Pump: ON");
    }
    else
    {
        display.println("Pump: OFF");
    }


    display.display();
}


// =====================================================
// CHECK SYSTEM DISPLAY
// =====================================================

void displayCheckSystem()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);


    display.setTextSize(2);

    display.setCursor(0, 0);
    display.println("CHECK");

    display.setCursor(0, 18);
    display.println("SYSTEM");


    display.setTextSize(1);

    display.setCursor(0, 42);
    display.println("Not watered");

    display.setCursor(0, 52);
    display.println("for 3+ days");


    display.display();
}


// =====================================================
// START PUMP
// =====================================================

void startPump()
{
    Serial.println();
    Serial.println("PUMP ON");


    // Relay is active LOW
    digitalWrite(pumpPin, LOW);


    pumpRunning = true;


    // Record when pump started
    pumpStartTime = millis();
}


// =====================================================
// STOP PUMP
// =====================================================

void stopPump()
{
    Serial.println("PUMP OFF");
    Serial.println("Starting soak period...");


    // Turn relay off
    digitalWrite(pumpPin, HIGH);


    pumpRunning = false;


    // Record beginning of soak period
    soakStartTime = millis();
}


// =====================================================
// SETUP
// =====================================================

void setup()
{
    // Start Serial Monitor
    Serial.begin(115200);


    // Pump output
    pinMode(pumpPin, OUTPUT);


    // Make sure pump starts OFF
    digitalWrite(pumpPin, HIGH);


    // Start DHT11
    dht.begin();


    // Start OLED
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            0x3C))
    {
        Serial.println("OLED FAILED!");

        while (true)
        {
        }
    }


    // Welcome screen
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);

    display.setCursor(0, 0);
    display.println("Welcome");

    display.display();


    delay(3000);


    // Start the 3-day timer
    systemStartTime = millis();


    // Take first sensor reading
    readSensors();
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
    unsigned long currentTime = millis();


    // =================================================
    // READ SENSORS EVERY 2 SECONDS
    // =================================================

    if (currentTime - lastSensorRead >= checkInterval)
    {
        lastSensorRead = currentTime;

        readSensors();
    }


    // =================================================
    // CHECK 3-DAY WARNING
    // =================================================

    bool checkSystem = false;


    // Plant has never reached 80% since startup
    if (!Watered)
    {
        if (currentTime - systemStartTime >= threedays)
        {
            checkSystem = true;
        }
    }

    // Plant has previously been successfully watered
    else
    {
        if (currentTime - lastWateredTime >= threedays)
        {
            checkSystem = true;
        }
    }


    // =================================================
    // WAITING FOR SOIL TO REACH 5%
    // =================================================

    if (!wateringMode)
    {
        // Soil is dry enough to begin watering
        if (soilMoisture <= DRY_LEVEL)
        {
            Serial.println();
            Serial.println("========================");
            Serial.println("SOIL REACHED 5%");
            Serial.println("STARTING WATERING");
            Serial.println("========================");


            wateringMode = true;


            startPump();
        }
    }


    // =================================================
    // WATERING MODE
    // =================================================

    if (wateringMode)
    {

        // =============================================
        // PUMP CURRENTLY RUNNING
        // =============================================

        if (pumpRunning)
        {
            displayData("Watering Plant");


            // Has pump run for 1.5 seconds?
            if (currentTime - pumpStartTime >= pumpRunTime)
            {
                stopPump();
            }
        }


        // =============================================
        // PUMP OFF - SOAKING PERIOD
        // =============================================

        else
        {
            displayData("Soaking...");


            // Wait for the full soak time before deciding
            // whether more water is required
            if (currentTime - soakStartTime >= soakTime)
            {

                // Take a fresh moisture reading
                readSensors();


                Serial.println();
                Serial.print("Moisture after soaking: ");
                Serial.print(soilMoisture);
                Serial.println("%");


                // =====================================
                // TARGET REACHED
                // =====================================

                if (soilMoisture >= WET_LEVEL)
                {
                    Serial.println("========================");
                    Serial.println("SOIL REACHED 80%");
                    Serial.println("WATERING COMPLETE");
                    Serial.println("========================");


                    wateringMode = false;

                    pumpRunning = false;


                    // Make sure pump is OFF
                    digitalWrite(pumpPin, HIGH);


                    // Plant successfully watered
                    Watered = true;


                    // Reset 3-day timer
                    lastWateredTime = currentTime;


                    displayData("Watering Complete");
                }


                // =====================================
                // STILL BELOW 80%
                // =====================================

                else
                {
                    Serial.println(
                        "Below 80% - watering again"
                    );


                    startPump();
                }
            }
        }
    }


    // =================================================
    // NORMAL DISPLAY WHILE WAITING FOR NEXT 5%
    // =================================================

    if (!wateringMode)
    {
        if (checkSystem)
        {
            displayCheckSystem();
        }

        else
        {
            displayData("Waiting for 5%");
        }
    }
}




