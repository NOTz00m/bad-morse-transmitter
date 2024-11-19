#include <Arduino.h>
#include "morse_mapping.h"

#define LED_PIN 13
#define BUZZER_PIN 12 // Pin for the piezo buzzer

// Morse code timings (in ms)
int DOT_DURATION = 200;
int DASH_DURATION = DOT_DURATION * 3;
int SYMBOL_PAUSE = DOT_DURATION;      // Pause between dots and dashes
int LETTER_PAUSE = DOT_DURATION * 3;
int WORD_PAUSE = DOT_DURATION * 7;

// Default values for reset
const int DEFAULT_DOT_DURATION = 200;
const int DEFAULT_DASH_DURATION = DEFAULT_DOT_DURATION * 3;
const int DEFAULT_SYMBOL_PAUSE = DEFAULT_DOT_DURATION;
const int DEFAULT_LETTER_PAUSE = DEFAULT_DOT_DURATION * 3;
const int DEFAULT_WORD_PAUSE = DEFAULT_DOT_DURATION * 7;

// Flag to control the stop functionality
volatile bool stopTransmission = false;

// Store the last Morse transmission
String lastMorseTransmission = "";

// Function prototypes
void transmitMorse(String text);
String getMorseCode(char c);
void blinkLED(int duration);
void playTone(int duration);
void handleSerialCommand(String command);
void printTimings();
void resetToDefault();
bool checkStopFlag();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter text to transmit as Morse code:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove leading spaces

    if (input == "RESET") {
      resetToDefault();
    } else if (input == "STOP") {
      stopTransmission = true;
      Serial.println("Morse code transmission stopped.");
    } else if (input == "TIMINGS") {
      printTimings();
    } else if (input == "LAST") {
      Serial.print("Last Morse Transmission: ");
      Serial.println(lastMorseTransmission);
    } else if (input.startsWith("SET ")) {
      handleSerialCommand(input); // Handle config commands
    } else {
      input.toUpperCase();
      stopTransmission = false; // Reset stop flag before starting new transmission
      lastMorseTransmission = ""; // Clear previous transmission
      transmitMorse(input);
    }
  }
}

void transmitMorse(String text) {
  for (int i = 0; i < text.length(); i++) {
    if (checkStopFlag()) return;

    char c = text.charAt(i);

    if (c == ' ') {
      if (checkStopFlag()) return;
      lastMorseTransmission += " / "; // Represent space between words
      delay(WORD_PAUSE);
    } else {
      String morse = getMorseCode(c);
      if (morse != "") {
        lastMorseTransmission += morse + " ";
        for (int j = 0; j < morse.length(); j++) {
          if (checkStopFlag()) return;
          if (morse[j] == '.') {
            blinkLED(DOT_DURATION);
            playTone(DOT_DURATION);
          } else if (morse[j] == '-') {
            blinkLED(DASH_DURATION);
            playTone(DASH_DURATION);
          }
          if (j < morse.length() - 1 && !checkStopFlag()) {
            delay(SYMBOL_PAUSE);
          }
        }
        if (!checkStopFlag()) delay(LETTER_PAUSE);
      } else {
        Serial.print("Unsupported character: ");
        Serial.println(c);
      }
    }
  }
}

void blinkLED(int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    if (checkStopFlag()) return;
    digitalWrite(LED_PIN, HIGH);
  }
  digitalWrite(LED_PIN, LOW);
}

void playTone(int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    if (checkStopFlag()) return;
    tone(BUZZER_PIN, 1000); // 1000 Hz tone
  }
  noTone(BUZZER_PIN);
}

String getMorseCode(char c) {
  int index = morseCodeChars.indexOf(c);
  if (index != -1) {
    return morseCodeMap[index];
  }
  return "";
}

void handleSerialCommand(String command) {
  if (command.startsWith("SET DOT ")) {
    DOT_DURATION = command.substring(8).toInt();
    DASH_DURATION = DOT_DURATION * 3;
    SYMBOL_PAUSE = DOT_DURATION;
    LETTER_PAUSE = DOT_DURATION * 3;
    WORD_PAUSE = DOT_DURATION * 7;
    Serial.println("Dot duration updated.");
  } else if (command.startsWith("SET DASH ")) {
    DASH_DURATION = command.substring(9).toInt();
    Serial.println("Dash duration updated.");
  } else if (command.startsWith("SET SYMBOL ")) {
    SYMBOL_PAUSE = command.substring(11).toInt();
    Serial.println("Symbol pause updated.");
  } else if (command.startsWith("SET LETTER ")) {
    LETTER_PAUSE = command.substring(11).toInt();
    Serial.println("Letter pause updated.");
  } else if (command.startsWith("SET WORD ")) {
    WORD_PAUSE = command.substring(9).toInt();
    Serial.println("Word pause updated.");
  } else {
    Serial.println("Unknown command.");
  }
}

void printTimings() {
  Serial.print("DOT_DURATION: ");
  Serial.println(DOT_DURATION);
  Serial.print("DASH_DURATION: ");
  Serial.println(DASH_DURATION);
  Serial.print("SYMBOL_PAUSE: ");
  Serial.println(SYMBOL_PAUSE);
  Serial.print("LETTER_PAUSE: ");
  Serial.println(LETTER_PAUSE);
  Serial.print("WORD_PAUSE: ");
  Serial.println(WORD_PAUSE);
}

void resetToDefault() {
  DOT_DURATION = DEFAULT_DOT_DURATION;
  DASH_DURATION = DEFAULT_DASH_DURATION;
  SYMBOL_PAUSE = DEFAULT_SYMBOL_PAUSE;
  LETTER_PAUSE = DEFAULT_LETTER_PAUSE;
  WORD_PAUSE = DEFAULT_WORD_PAUSE;
  Serial.println("All values reset to default.");
}

bool checkStopFlag() {
  if (stopTransmission) {
    Serial.println("Transmission stopped.");
    return true;
  }
  return false;
}
