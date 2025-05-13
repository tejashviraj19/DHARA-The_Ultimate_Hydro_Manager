#include <FirebaseESP8266.h>
#include <ArduinoJson.h>

#define FIREBASE_HOST "your-firebase-project.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-authentication"

FirebaseData firebaseData;
StaticJsonDocument<200> jsonDocument;

void setup() {
  Serial.begin(9600);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if (Serial.available()) {
    String jsonString = Serial.readStringUntil('\n');

    DeserializationError error = deserializeJson(jsonDocument, jsonString);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      return;
    }

    float phValue = jsonDocument["pH"];
    float tdsValue = jsonDocument["TDS"];
    float humidity = jsonDocument["Humidity"];
    float temperature = jsonDocument["Temperature"];

    // Update Firebase with sensor data
    Firebase.setString(firebaseData, "/sensorData/ph", String(phValue));
    Firebase.setString(firebaseData, "/sensorData/tds", String(tdsValue));
    Firebase.setString(firebaseData, "/sensorData/humidity", String(humidity));
    Firebase.setString(firebaseData, "/sensorData/temperature", String(temperature));

    // Control relays based on commands from website
    if (jsonDocument.containsKey("relay1")) {
      int relay1State = jsonDocument["relay1"];
      // Control relay connected to motor 1
    }
    if (jsonDocument.containsKey("relay2")) {
      int relay2State = jsonDocument["relay2"];
      // Control relay connected to motor 2
    }
    if (jsonDocument.containsKey("relay3")) {
      int relay3State = jsonDocument["relay3"];
      // Control relay connected to motor 3
    }
  }

  delay(1000);
}
