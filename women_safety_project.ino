#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// GSM
SoftwareSerial gsm(9, 10);

// GPS
SoftwareSerial gpsSerial(4, 3);
TinyGPSPlus gps;

// Pins
int buttonPin = 2;
int buzzer = 8;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);

  Serial.begin(9600);
  gsm.begin(9600);
  gpsSerial.begin(9600);

  delay(3000);
}

void loop() {

  // Read GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Button pressed
  if (digitalRead(buttonPin) == LOW) {

    digitalWrite(buzzer, HIGH);

    float lat = 0.0;
    float lng = 0.0;

    // Check GPS
    if (gps.location.isValid()) {
      lat = gps.location.lat();
      lng = gps.location.lng();
    }

    sendSMS(lat, lng);

    delay(5000);

    digitalWrite(buzzer, LOW);
  }
}

void sendSMS(float lat, float lng) {

  gsm.println("AT+CMGF=1");
  delay(1000);

  gsm.println("AT+CMGS=\"+91XXXXXXXXXX\"");
  delay(1000);

  gsm.println("EMERGENCY ALERT!");
  gsm.println("I need help.");

  gsm.print("Location: ");
  gsm.print("https://maps.google.com/?q=");
  gsm.print(lat, 6);
  gsm.print(",");
  gsm.println(lng, 6);

  delay(1000);

  gsm.write(26);

  delay(3000);
}