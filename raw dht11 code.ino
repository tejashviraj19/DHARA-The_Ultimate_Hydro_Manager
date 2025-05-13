#include <DHT.h>

#define DHTPIN 7 // You can choose any digital pin from 2 to 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud rate
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  int chk = dht.read11(DHTPIN);

  if (chk == DHT::DHT_SUCCESS) {
    float temp = dht.temperature;
    float hum = dht.humidity;

    // Print temperature (can be Celsius or Fahrenheit based on dht.readTemperature())
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println("°C");

    // Print humidity
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println("%");
  } else {
    // Error handling (optional)
    Serial.println("Error reading DHT sensor!");
  }

  // Delay between readings (adjust if needed)
  delay(2000);
}
