#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// WiFi credentials
#define WIFI_SSID "MyWifi"
#define WIFI_PASSWORD "123456789"

// Firebase credentials
#define API_KEY "Use your API key"
#define DATABASE_URL "Your database URL"
#define USER_EMAIL "your firebase email id"
#define USER_PASSWORD "your password"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Relay module pin
const int relayPin = D1;

void setup()
{
  pinMode(relayPin, OUTPUT); // Set relay pin as output

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
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

void loop()
{
  if (Firebase.ready())
  {
    int relayState;
    if (Firebase.RTDB.getInt(&fbdo, "/relay/state", &relayState))
    {
      digitalWrite(relayPin, relayState);
    }
    else
    {
      Serial.println(fbdo.errorReason().c_str());
    }
  }
}
