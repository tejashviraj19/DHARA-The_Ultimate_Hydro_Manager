#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// WiFi credentials
#define WIFI_SSID "MyWifi"
#define WIFI_PASSWORD "123456789"

// Firebase credentials
#define API_KEY "AIzaSyCxu076kqDUD4mqy_NzBrhZhxiBFn9hqCs"
#define DATABASE_URL "https://nodemcu-e8e90-default-rtdb.firebaseio.com/"
#define USER_EMAIL "imrishabh9628@gmail.com"
#define USER_PASSWORD "firebase123"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;


void setup() {

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
}

void loop() {


  if (Serial.available() > 0) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, Serial);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      return;
    }

    // Check if the JSON object contains the expected fields
    if (doc.containsKey("humidity") && doc.containsKey("temperature")) {
      float humidity = doc["humidity"];
      float temperature = doc["temperature"];

      Serial.print("Humidity: ");
      Serial.println(humidity);
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println("°C");


      if (Firebase.ready()) {

        if (Firebase.RTDB.setFloat(&fbdo, "DHT/humidity", humidity)) {
        } else {
          Serial.print("failed to send humidity data to firebase");
        }


        if (Firebase.RTDB.setFloat(&fbdo, "DHT/temperature", temperature)) {
        } else {
          Serial.print("failed to send temperature data to firebase");
        }






      } else {
        Serial.println("JSON object is missing required fields");
      }
    }
  }
}
