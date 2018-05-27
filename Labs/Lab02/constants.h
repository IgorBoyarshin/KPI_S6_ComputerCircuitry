#ifndef CONSTANTS_H
#define CONSTANTS_H

const static unsigned char SYMBOL_LENGTH = 6;
const static unsigned char FIELD_SIZE = (1 << SYMBOL_LENGTH); // 2^6
const static unsigned char CORRECTING_MISTAKES = 2;
const static unsigned char CONTROL_SYMBOLS = 2 * CORRECTING_MISTAKES;
// const static unsigned char INFORMATION_SYMBOLS = 3; // <= 2^m - 1 - 2*H
// const static unsigned char MESSAGE_LENGTH = CONTROL_SYMBOLS + INFORMATION_SYMBOLS;

#endif
