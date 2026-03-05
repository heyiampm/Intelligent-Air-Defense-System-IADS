#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Pin
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 13;
const int redLED = 4;
const int yellowLED = 3;
const int greenLED = 2;
const int servoPin = 11;

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Variable
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  myServo.attach(servoPin);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  
  lcd.print("Radar System");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // 0 to 180 degree scan
  for (int i = 0; i <= 180; i++) {
    scanAndCheck(i);
  }
  // 180 to 0 degree scaning
  for (int i = 180; i >= 0; i--) {
    scanAndCheck(i);
  }
}

void scanAndCheck(int angle) {
  myServo.write(angle);
  delay(30); 
  
  distance = getDistance();
  
  // Radar send to data on pc
  Serial.print(angle); Serial.print(","); Serial.print(distance); Serial.print(".");

  // Lock mode- terget 15 cm
  if (distance < 15 && distance > 1) {
    lockSystem(angle);
  } else {
    updateDisplay(distance, angle);
    noTone(buzzer);
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 25000); 
  int d = duration * 0.034 / 2;
  return (d == 0) ? 400 : d;
}

void lockSystem(int angle) {
  // stop the loop or ditect object
  while (getDistance() < 15) {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    
    lcd.setCursor(0, 0); lcd.print("TARGET LOCKED!  ");
    lcd.setCursor(0, 1); lcd.print("Angle: "); lcd.print(angle); lcd.print(" deg    ");

    //Buzzer Sound
    tone(buzzer, 1200);
    delay(100);
    tone(buzzer, 800);
    delay(100);
  }
  // move the object red light is low 
  digitalWrite(redLED, LOW);
  noTone(buzzer);
}

void updateDisplay(int d, int angle) {
  lcd.setCursor(0, 0); lcd.print("Scanning...     ");
  lcd.setCursor(0, 1); lcd.print("D:"); lcd.print(d); lcd.print("cm  A:"); lcd.print(angle); lcd.print("   ");

  if (d >= 15 && d < 40) { // yellow zone
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
  } else if (d >= 40) { // green zone
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
}