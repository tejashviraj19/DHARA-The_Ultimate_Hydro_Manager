#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ArduinoJson.h>


#define sensorPinTDS A1
#define SensorPinPH A2
#define DHTPIN 9
DHT dht(DHTPIN, DHT11);  // Assuming DHT11 sensor


LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


const int motorPin1 = 5;  // Pin connected to the relay module controlling motor 1
const int motorPin2 = 6;  // Pin connected to the relay module controlling motor 2
const int motorPin3 = 7;  // Pin connected to the relay module controlling motor 3

unsigned long lastMotor2RunTime = 0;


float temperature, humidity;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(motorPin1, OUTPUT);  // Initialize motorPin1 as output
  pinMode(motorPin2, OUTPUT);  // Initialize motorPin2 as output
  pinMode(motorPin3, OUTPUT);  // Initialize motorPin3 as output
  Serial.println("Program started");
}

void loop() {
  StaticJsonDocument<200> doc;

  // Read sensor data
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();


  // Read TDS value
  int sensorValueTDS = analogRead(sensorPinTDS);
  float VoltageTDS = sensorValueTDS * 5.0 / 1024.0;
  float tdsValue = (133.42 / VoltageTDS * VoltageTDS * VoltageTDS - 255.86 * VoltageTDS * VoltageTDS + 857.39 * VoltageTDS) * 0.5;


  // Read pH value
  int buf[10], temp;
  unsigned long int avgValue = 0;
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(SensorPinPH);
    delay(10);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  for (int i = 2; i < 8; i++) {
    avgValue += buf[i];
  }
  float phValue = (float)avgValue * 5.0 / 1024.0 / 6.0;
  phValue = 3.5 * phValue;



  // Check for invalid sensor readings (optional)
  if (isnan(humidity) || isnan(temperature) || isnan(phValue) || isnan(tdsValue)) {
    Serial.println("Failed to read from sensor!");
  }

  // Assign data to JSON object
  doc.clear();
  doc["humidity"] = humidity;
  doc["temperature"] = temperature;
  doc["phValue"] = phValue;
  doc["tdsValue"] = tdsValue;



  // Control relay 1 based on pH value
  if (phValue < 6.5) {
    doc["relay1"] = 1;  // Turn on relay 1
  } else if (phValue >= 7.5) {
    doc["relay1"] = 0;  // Turn off relay 1
  }


  // Control relay 2 to run every 1 hour for 5 seconds
  if (millis() - lastMotor2RunTime >= 3600000) {
    doc["relay2"] = 1;             // Turn on relay 2
    delay(5000);                   // Run relay 2 for 5 seconds
    doc["relay2"] = 0;             // Turn off relay 2
    lastMotor2RunTime = millis();  // Update last run time
  } else {
    // Set relay 2 to 0 if not within the hourly interval (assuming you have a way to set values in doc)
    doc["relay2"] = 0; 
  }


  // Control relay 3 based on temperature
  if (temperature > 33) {
    doc["relay3"] = 1;  // Turn on relay 3
  } else {
    doc["relay3"] = 0;  // Turn off relay 3
  }


  // Convert JSON object to string
  String jsonString;
  serializeJson(doc, jsonString);



// Display sensor values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS: ");
  lcd.print(tdsValue);
  lcd.setCursor(8, 0);
  lcd.print("pH: ");
  lcd.print(phValue, 2);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.setCursor(8, 1);
  lcd.print("H: ");
  lcd.print(humidity);



  // Send data to NodeMCU
  Serial.println(jsonString);
  delay(1000);  // Adjust delay as needed
}
