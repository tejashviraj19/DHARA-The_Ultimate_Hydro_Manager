#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SERIAL Serial
#define sensorPinTDS A1
#define sensorPin A0
#define SensorPinPH A2

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

const int pumpPin = 4; // Pin connected to the relay module controlling the pump
unsigned long alertStartTime = 0;
const unsigned long alertDuration = 3000; // Duration of alert display in milliseconds (3 seconds)
bool alertDisplayed = false;

void setup() {
  SERIAL.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(13, OUTPUT); // Assuming pin 13 is used for a pump, adjust accordingly
  pinMode(pumpPin, OUTPUT); // Initialize pump pin as output
}

void loop() {
  // Read TDS value
  int sensorValueTDS = analogRead(sensorPinTDS);
  float VoltageTDS = sensorValueTDS * 5.0 / 1024.0;
  float tdsValue = (133.42 / VoltageTDS * VoltageTDS * VoltageTDS - 255.86 * VoltageTDS * VoltageTDS + 857.39 * VoltageTDS) * 0.5;
  SERIAL.print("TDS Value = ");
  SERIAL.print(tdsValue);
  SERIAL.println(" ppm");

  // Read turbidity value
  int sensorValueTur = analogRead(sensorPin);
  int turbidityValue = map(sensorValueTur, 0, 640, 100, 0);
  SERIAL.print("Turbidity = ");
  SERIAL.println(turbidityValue);

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
  SERIAL.print("pH value = ");
  SERIAL.print(phValue, 2);
  SERIAL.println(" ");

  // Display sensor values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turbidity: ");
  lcd.print(turbidityValue);
  lcd.setCursor(0, 1);
  lcd.print("TDS: ");
  lcd.print(tdsValue);
  lcd.setCursor(8, 1);
  lcd.print("pH: ");
  lcd.print(phValue, 2);

  // Check if pH is below 6.5 and take action
  if (phValue < 7.5) {
    // Turn on pump
    digitalWrite(pumpPin, HIGH); // Assuming HIGH starts the pump, adjust if necessary
    // Display alert on Serial Monitor
    SERIAL.println("Alert: pH low!");
    // Display alert on LCD
    /*
    if (!alertDisplayed) {
      lcd.setCursor(0, 2);
      lcd.print("Alert: pH low!");
      alertStartTime = millis();
      alertDisplayed = true;
    } else {
      if (millis() - alertStartTime >= alertDuration) {
        // Clear alert message on LCD after duration
        lcd.setCursor(0, 2);
        lcd.print("                ");
        alertDisplayed = false;
      }
    }
    */
  } else {
    // Turn off pump
    digitalWrite(pumpPin, LOW); // Assuming LOW stops the pump, adjust if necessary
    // Clear alert message on LCD
    /*
    if (alertDisplayed) {
      lcd.setCursor(0, 2);
      lcd.print("                ");
      alertDisplayed = false;
    }
    */
  }

  // Check if TDS value is above a certain threshold and take action
  if (tdsValue > 1000) {
    // Display alert on Serial Monitor
    SERIAL.println("Alert: High TDS!");
    // Display alert on LCD
    /*
    if (!alertDisplayed) {
      lcd.setCursor(0, 2);
      lcd.print("Alert: High TDS!");
      alertStartTime = millis();
      alertDisplayed = true;
    } else {
      if (millis() - alertStartTime >= alertDuration) {
        // Clear alert message on LCD after duration
        lcd.setCursor(0, 2);
        lcd.print("                ");
        alertDisplayed = false;
      }
    }
    */
  }

  // Check if turbidity value is above a certain threshold and take action
  if (turbidityValue > 50) {
    // Display alert on Serial Monitor
    SERIAL.println("Alert: High Turbidity!");
    // Display alert on LCD
    /*
    if (!alertDisplayed) {
      lcd.setCursor(0, 1);
      lcd.print("Alert:High Turbidity!");
      alertStartTime = millis();
      alertDisplayed = true;
    } else {
      if (millis() - alertStartTime >= alertDuration) {
        // Clear alert message on LCD after duration
        lcd.setCursor(0, 1);
        lcd.print("                ");
        alertDisplayed = false;
      }
    }
    */
  }

  // Delay for 1 second before looping again
  delay(1000);
}
