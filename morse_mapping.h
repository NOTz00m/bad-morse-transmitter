/*
 * morse_mapping.h
 * 
 * International Morse Code mapping for alphanumeric characters and symbols.
 * Supports A-Z, 0-9, and common punctuation marks.
 * 
 * Morse Code Standard:
 * - Dot (dit): Short signal
 * - Dash (dah): Long signal (3x dot duration)
 * - Intra-character gap: 1x dot duration
 * - Inter-character gap: 3x dot duration
 * - Word gap: 7x dot duration
 */

#ifndef MORSE_MAPPING_H
#define MORSE_MAPPING_H

#include <Arduino.h>

// Morse code patterns for supported characters
const String morseCodeMap[] = {
  // Letters A-Z
  ".-",    "-...",  "-.-.",  "-..",   ".",     "..-.",  "--.",   "....",  // A-H
  "..",    ".---",  "-.-",   ".-..",  "--",    "-.",    "---",   ".--.",  // I-P
  "--.-",  ".-.",   "...",   "-",     "..-",   "...-",  ".--",   "-..-",  // Q-X
  "-.--",  "--..",                                                         // Y-Z
  
  // Digits 0-9
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", // 0-7
  "---..", "----.",                                                        // 8-9
  
  // Punctuation and special characters
  ".--.-.",  // @ (at sign)
  "--..--",  // , (comma)
  "..--..",  // ? (question mark)
  ".-..-.",  // " (quotation mark)
  "-.-.--",  // ! (exclamation mark)
  "---...",  // : (colon)
  "-.--.",   // ( (opening parenthesis)
  "-.--.-",  // ) (closing parenthesis)
  ".-...",   // & (ampersand)
  "-....-",  // - (hyphen/minus)
  "..--.-",  // _ (underscore)
  ".-.-.-",  // . (period/full stop)
  "-..-.",   // / (forward slash)
  "-.-.-."   // ; (semicolon)
};

// Character set corresponding to morse code patterns
// Must maintain exact 1:1 correspondence with morseCodeMap array
const String morseCodeChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@,?\"!:()&-_./;";

// Compile-time validation
static_assert(
  sizeof(morseCodeMap) / sizeof(morseCodeMap[0]) == 49,
  "Morse code map size mismatch - verify character count"
);

#endif // MORSE_MAPPING_H