#include <SPI.h>

const int CS = 15;
const int buttonT1 = 32;
const int buttonT2 = 2;
const int buttonT4 = 14;

int i = 0;
int c = 0;
int buttonT2State = LOW;
int lastButtonT2State = LOW;
unsigned long buttonT2PressedTime = 0;

int buttonT1State = LOW;
int lastButtonT1State = LOW;
unsigned long buttonT1PressedTime = 0;


const unsigned long debounceDelay = 50;
bool buttonT1Pressed = false;
bool buttonT2Pressed = false;
bool buttonT4Pressed = false;
bool countdownStarted = false;

unsigned long countdownDuration = 1000;  // Dauer des Countdowns in Millisekunden
unsigned long previousTime = 0;

void setup() {

  SPI.begin();
  pinMode(CS, OUTPUT);
  pinMode(buttonT1, INPUT);
  pinMode(buttonT2, INPUT);
  pinMode(buttonT4, INPUT);
  
}

void loop() {
  char myArray[10] = {0x7E, 0x0A, 0xB6, 0x9E, 0xCA, 0xDC, 0xFC, 0x0E, 0xFE, 0xDE};

  char r = myArray[i];
  char l = myArray[c];

  //---------------------------------------T1--------------------------------------------

  buttonT1State = digitalRead(buttonT1);

  if (buttonT1State == HIGH && lastButtonT1State == LOW) {
    buttonT1PressedTime = millis();
    buttonT1Pressed = true;
  }

  if (buttonT1State == LOW && lastButtonT1State == HIGH && millis() - buttonT1PressedTime > debounceDelay) {
    if (buttonT1Pressed) {
      i--;
      buttonT1Pressed = false;
    }
  }

  //---------------------------------------T2--------------------------------------------

  buttonT2State = digitalRead(buttonT2);

  if (buttonT2State == HIGH && lastButtonT2State == LOW) {
    buttonT2PressedTime = millis();
    buttonT2Pressed = true;
  }

  if (buttonT2State == LOW && lastButtonT2State == HIGH && millis() - buttonT2PressedTime > debounceDelay) {
    if (buttonT2Pressed) {
      i++;
      buttonT2Pressed = false;
    }
  }

  //---------------------------------------T4--------------------------------------------

  if (digitalRead(buttonT4) == HIGH && !buttonT4Pressed) {
    buttonT4Pressed = true;
    countdownStarted = true;
  }

  //Aktuellen Zustand der Tasten Speichern
  lastButtonT2State = buttonT2State;
  lastButtonT1State = buttonT1State;

  //Zahlen erhöhen und verringern
  if (i == 10) {
    i = 0;
    c++;
  } else if (i == -1) {
    i = 9;
    c--;
  }

  //Countdown
  if (countdownStarted) {
    unsigned long currentTime = millis();

    if (currentTime - previousTime >= countdownDuration) {
      i--;
      previousTime = currentTime;
    }


    if (i == 0 && c == 0) {
      buttonT4Pressed = false;
      countdownStarted = false;  // Countdown beenden
    }
  }

  //Kommunikation über den SPI 
  digitalWrite(CS, LOW);
  SPI.transfer(r);
  SPI.transfer(l);
  digitalWrite(CS, HIGH);

  delay(100);
}