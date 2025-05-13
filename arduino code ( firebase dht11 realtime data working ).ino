#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 4
DHT dht(DHTPIN, DHT11);  // Assuming DHT11 sensor

float temp, hum;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("Program started");
}

void loop() {
  StaticJsonDocument<200> doc;

  // Read sensor data
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  // Check for invalid sensor readings (optional)
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  // Assign data to JSON object
  doc.clear();
  doc["humidity"] = hum;
  doc["temperature"] = temp;

  // Convert JSON object to string
  String jsonString;
  serializeJson(doc, jsonString);

  // Send data to NodeMCU
  Serial.println(jsonString);
  delay(1000);  // Adjust delay as needed
}
