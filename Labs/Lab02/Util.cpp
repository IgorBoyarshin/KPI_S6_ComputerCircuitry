#include "Util.h"


Polynom Util::encode(const std::string& message) {
    Polynom symbols;
    for (unsigned int i = 0; i < message.size(); i++) {
        symbols.push_back(getNumber(message[i]));
    }

    return symbols;
}


std::string Util::decode(const Polynom& symbols) {
    std::string message(symbols.size(), '*');
    for (unsigned int i = 0; i < symbols.size(); i++) {
        message[i] = getChar(symbols[i]);
    }

    return message;
}


char Util::getChar(const Number& number) {
    char c = 0;
    c = number.id + 'A';
    if ('A' <= c && c <= 'Z') {
        return c;
    }
    c = number.id + 'a' - 26;
    if ('a' <= c && c <= 'z') {
        return c;
    }
    c = number.id + '0' - 2 * 26;
    if ('0' <= c && c <= '9') {
        return c;
    }
    if (number.id == 62+0) {
        return ' ';
    }
    if (number.id == 62+1) {
        return ',';
    }

    return '*';
}


Number Util::getNumber(char c) {
    if ('A' <= c && c <= 'Z') {
        return Number(c - 'A');
    } else if ('a' <= c && c <= 'z') {
        return Number(26 + c - 'a');
    } else if ('0' <= c && c <= '9') {
        return Number(2 * 26 + c - '0');
    } else if (' ' == c) {
        return Number(62 + 0);
    } else if (',' == c) {
        return Number(62 + 1);
    }

    return Number::getZero();
}
