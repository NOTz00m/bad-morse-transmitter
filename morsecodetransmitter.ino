/*
 * Morse Code Transmitter
 * 
 * A versatile Morse code transmission system with LED and audio output.
 * Supports configurable timing parameters and serial control interface.
 * 
 * Hardware Requirements:
 * - LED on pin 13
 * - Piezo buzzer on pin 12
 * 
 * Commands:
 * - Type text to transmit as Morse code
 * - RESET: Restore default timing settings
 * - STOP: Halt current transmission
 * - TIMINGS: Display current timing values
 * - LAST: Show the last transmitted Morse sequence
 * - SET [PARAMETER] [VALUE]: Configure timing (DOT, DASH, SYMBOL, LETTER, WORD)
 */

#include <Arduino.h>
#include "morse_mapping.h"

// Hardware pin definitions
const uint8_t LED_PIN = 13;
const uint8_t BUZZER_PIN = 12;

// Morse timing parameters (in milliseconds)
struct MorseTiming {
  int dot;
  int dash;
  int symbolPause;
  int letterPause;
  int wordPause;
};

MorseTiming timing = {200, 600, 200, 600, 1400};
const MorseTiming DEFAULT_TIMING = {200, 600, 200, 600, 1400};

// Transmission control
volatile bool stopRequested = false;
String lastTransmission = "";

// Buzzer frequency for tone generation
const uint16_t BUZZER_FREQUENCY = 1000;

// Function declarations
void transmitMorse(const String& text);
String getMorseCode(char character);
void signalDot();
void signalDash();
void outputSignal(int duration);
void handleCommand(const String& command);
void displayTimings();
void resetTimings();
bool shouldStop();
void updateRelatedTimings();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println(F("================================="));
  Serial.println(F("   Morse Code Transmitter v2.0   "));
  Serial.println(F("================================="));
  Serial.println(F("Ready to transmit."));
  Serial.println(F("Type text or command (RESET, STOP, TIMINGS, LAST)"));
  Serial.println();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.length() == 0) return;
    
    // Handle special commands
    if (input.equalsIgnoreCase("RESET")) {
      resetTimings();
    } 
    else if (input.equalsIgnoreCase("STOP")) {
      stopRequested = true;
      Serial.println(F("Stop signal sent."));
    } 
    else if (input.equalsIgnoreCase("TIMINGS")) {
      displayTimings();
    } 
    else if (input.equalsIgnoreCase("LAST")) {
      Serial.print(F("Last transmission: "));
      Serial.println(lastTransmission.length() > 0 ? lastTransmission : "None");
    } 
    else if (input.startsWith("SET ")) {
      handleCommand(input);
    } 
    else {
      // Transmit as Morse code
      input.toUpperCase();
      stopRequested = false;
      lastTransmission = "";
      Serial.print(F("Transmitting: "));
      Serial.println(input);
      transmitMorse(input);
      if (!stopRequested) {
        Serial.println(F("Transmission complete."));
      }
    }
  }
}

void transmitMorse(const String& text) {
  for (size_t i = 0; i < text.length(); i++) {
    if (shouldStop()) return;
    
    char c = text.charAt(i);
    
    if (c == ' ') {
      lastTransmission += "/ ";
      delay(timing.wordPause);
    } 
    else {
      String morse = getMorseCode(c);
      
      if (morse.length() > 0) {
        lastTransmission += morse + " ";
        
        for (size_t j = 0; j < morse.length(); j++) {
          if (shouldStop()) return;
          
          if (morse[j] == '.') {
            signalDot();
          } 
          else if (morse[j] == '-') {
            signalDash();
          }
          
          // Pause between symbols within a letter
          if (j < morse.length() - 1 && !shouldStop()) {
            delay(timing.symbolPause);
          }
        }
        
        // Pause between letters
        if (!shouldStop() && i < text.length() - 1 && text.charAt(i + 1) != ' ') {
          delay(timing.letterPause);
        }
      } 
      else {
        Serial.print(F("Warning: Unsupported character '"));
        Serial.print(c);
        Serial.println(F("' skipped."));
      }
    }
  }
}

void signalDot() {
  outputSignal(timing.dot);
}

void signalDash() {
  outputSignal(timing.dash);
}

void outputSignal(int duration) {
  unsigned long startTime = millis();
  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, BUZZER_FREQUENCY);
  
  while (millis() - startTime < duration) {
    if (shouldStop()) {
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
      return;
    }
  }
  
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
}

String getMorseCode(char character) {
  int index = morseCodeChars.indexOf(character);
  return (index != -1) ? morseCodeMap[index] : "";
}

void handleCommand(const String& command) {
  String cmd = command.substring(4);
  cmd.trim();
  
  int spaceIndex = cmd.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println(F("Error: Invalid SET command format."));
    Serial.println(F("Usage: SET [DOT|DASH|SYMBOL|LETTER|WORD] [value]"));
    return;
  }
  
  String parameter = cmd.substring(0, spaceIndex);
  int value = cmd.substring(spaceIndex + 1).toInt();
  
  if (value <= 0) {
    Serial.println(F("Error: Value must be positive."));
    return;
  }
  
  parameter.toUpperCase();
  
  if (parameter == "DOT") {
    timing.dot = value;
    updateRelatedTimings();
    Serial.print(F("Dot duration set to "));
  } 
  else if (parameter == "DASH") {
    timing.dash = value;
    Serial.print(F("Dash duration set to "));
  } 
  else if (parameter == "SYMBOL") {
    timing.symbolPause = value;
    Serial.print(F("Symbol pause set to "));
  } 
  else if (parameter == "LETTER") {
    timing.letterPause = value;
    Serial.print(F("Letter pause set to "));
  } 
  else if (parameter == "WORD") {
    timing.wordPause = value;
    Serial.print(F("Word pause set to "));
  } 
  else {
    Serial.println(F("Error: Unknown parameter."));
    Serial.println(F("Valid parameters: DOT, DASH, SYMBOL, LETTER, WORD"));
    return;
  }
  
  Serial.print(value);
  Serial.println(F(" ms"));
}

void updateRelatedTimings() {
  // Maintain standard Morse timing ratios when dot duration changes
  timing.dash = timing.dot * 3;
  timing.symbolPause = timing.dot;
  timing.letterPause = timing.dot * 3;
  timing.wordPause = timing.dot * 7;
  Serial.println(F("Related timings auto-adjusted to maintain standard ratios."));
}

void displayTimings() {
  Serial.println(F("--- Current Timing Settings ---"));
  Serial.print(F("Dot duration:    ")); Serial.print(timing.dot); Serial.println(F(" ms"));
  Serial.print(F("Dash duration:   ")); Serial.print(timing.dash); Serial.println(F(" ms"));
  Serial.print(F("Symbol pause:    ")); Serial.print(timing.symbolPause); Serial.println(F(" ms"));
  Serial.print(F("Letter pause:    ")); Serial.print(timing.letterPause); Serial.println(F(" ms"));
  Serial.print(F("Word pause:      ")); Serial.print(timing.wordPause); Serial.println(F(" ms"));
  Serial.println(F("-------------------------------"));
}

void resetTimings() {
  timing = DEFAULT_TIMING;
  Serial.println(F("Timings reset to defaults."));
  displayTimings();
}

bool shouldStop() {
  if (stopRequested) {
    Serial.println(F("Transmission halted by user."));
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    return true;
  }
  return false;
}