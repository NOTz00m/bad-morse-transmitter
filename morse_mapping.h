#ifndef MORSE_MAPPING_H
#define MORSE_MAPPING_H

// Morse code dictionary for letters, digits, and special characters
String morseCodeMap[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", // A-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", // 0-9
  ".--.-.", "--..--", "..--..", ".-..-.", "-.-.--", "---...", "-.--.", "-.--.-", ".-...", "-....-", ".--.-.", // Symbols: @, , , ?, ", !, :, (, ), &, -, @
  "..--.-", "-..-.", ".-.-.-", "-.-.-.", "/" // TODO: ADD SUPPORT FOR EXTENDED EUROPEAN LETTERS SUCH AS ACCENTS.
};

// Corresponding character set for Morse code mapping
String morseCodeChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@,?\"!:()&-_. /";

#endif
