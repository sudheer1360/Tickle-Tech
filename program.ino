#define BLYNK_TEMPLATE_ID "TMPL3fyDH50Cv"
#define BLYNK_TEMPLATE_NAME "MPU"
#define BLYNK_AUTH_TOKEN "8F7Ww6R0DaP9_4nzhsYTDxyC6cPCJdnk"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>

BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "123";
char pass[] = "1234567890";

const int rainSensorPin = 34; 
const int lightSensorPin = 32;
const int servoPin1 = 13;
const int servoPin2 = 12;
const int motorPin1 = 25;
const int motorPin2 = 26;
const int oneWireBus = 27;
const int buzzer=5;

Servo myServo1;
Servo myServo2; 

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass); 
  pinMode(buzzer,OUTPUT);
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
 sensors.begin();
}

void loop() {
  Blynk.run(); 
  checkRain();
  checkLight();
  checkTemperature();
}

void checkRain() {
  int rainValue = analogRead(rainSensorPin);
  Serial.print(rainValue);
  if (rainValue > 500) {
    Blynk.virtualWrite(V0,rainValue);
    Blynk.logEvent("Rain","Rain Detected Turn On the Servo");
    myServo1.write(90); 
    myServo2.write(90);
  }
  else{
    myServo1.write(0);
    myServo2.write(0);
    }
  }

void checkLight() {
  int lightValue = analogRead(lightSensorPin);
  Serial.print(lightValue);
  if (lightValue == 4095) {
    Blynk.virtualWrite(V1,lightValue);
    Blynk.logEvent("Object","Animlas Detected");
    digitalWrite(buzzer,HIGH);
  }
  else{
    digitalWrite(buzzer,LOW);
  }
}

// Function to check temperature
void checkTemperature() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  Blynk.virtualWrite(V2,temperature);
  Blynk.logEvent("High_temp","High Temperature is detected in water turn oning motors");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  if (temperature > 35) { 
    digitalWrite(motorPin1, HIGH); 
    digitalWrite(motorPin2, LOW); 
  } else {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }
}
