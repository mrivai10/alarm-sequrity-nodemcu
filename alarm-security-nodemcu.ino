#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266.h>

LiquidCrystal_I2C lcd (0x27, 16, 2);
const char* auth = ""; //token autentikasi blynk
const char* ssid = ""; //ssid wifi 
const char* password = ""; // password wifi


#define Sensor D0
#define LEDR D3
#define LEDG D4
#define Buzzer D5
bool alarmStatus = false;

WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, password);
  lcd.begin();
  Wire.begin(D2, D1);

  pinMode(Sensor, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");
  }

  Serial.println("");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  Serial.println("WiFi Connected");
  delay(1000);
  Serial.print("IP address: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Started");
  delay(3000);
  lcd.clear();

  Blynk.virtualWrite(V0, LOW);
}

BLYNK_WRITE(V0) {
  int pinValue = param.asInt();
  
  if (pinValue == 1) {
    alarmStatus = true;
  } else {
    alarmStatus = false;
  }
}

void loop() {
  Blynk.run();
  bool value = digitalRead(Sensor);

  if (alarmStatus) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarm: ON");
    
    if (value == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Motion Detected!");

      Blynk.logEvent("pirmotion","WARNING! Motion Detected!");
      WidgetLED LED(V1);
      LED.on();
    
      digitalWrite(LEDR, HIGH);
      digitalWrite(Buzzer, HIGH);
      digitalWrite(LEDG, LOW);
    }else{
      lcd.setCursor(0, 1);
      lcd.print("No Motion");

      WidgetLED LED(V1);
      LED.off();

      digitalWrite(LEDR, LOW);
      digitalWrite(Buzzer, LOW);
      digitalWrite(LEDG, HIGH);
    }
 }else{
    lcd.clear();
    lcd.print("Alarm: OFF");
    lcd.setCursor(0, 1);
    lcd.print("No Motion");
    
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
 }
 
  delay(500);
}
