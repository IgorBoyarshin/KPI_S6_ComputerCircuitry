#include "Number.h"


unsigned char Number::idByCode[FIELD_SIZE];
unsigned char Number::codeById[FIELD_SIZE];
bool Number::firstLaunch = true;


Number::Number() : id(FIELD_SIZE - 1) {}


Number::Number(unsigned char id) : id(id) {}


Number Number::operator+(const Number& number) const {
    return Number::add(*this, number);
}


Number Number::operator*(const Number& number) const {
    return Number::mul(*this, number);
}


Number Number::operator/(const Number& number) const {
    return Number::div(*this, number);
}


Number Number::operator^(unsigned int power) const {
    return Number::pow(*this, power);
}


unsigned char Number::getCode() const {
    return getCodeById(id);
}


Number Number::add(const Number& number1, const Number& number2) {
    const unsigned char code1 = getCodeById(number1.id);
    const unsigned char code2 = getCodeById(number2.id);

    return Number(getIdByCode(code1 ^ code2));
}


Number Number::pow(const Number& number1, unsigned int power) {
    Number res = Number(0);
    for (unsigned int i = 0; i < power; i++) {
        res = res * number1;
    }

    return res;
}


Number Number::mul(const Number& number1, const Number& number2) {
    if (number1.id == FIELD_SIZE - 1 || number2.id == FIELD_SIZE - 1) {
        return Number::getZero();
        // std::cout << "USAGE OF ZERO IN MULTIPLICATION FIELD. TERMINATING" << std::endl;
        // exit(-1);
    }
    const unsigned char id = (number1.id + number2.id) % (FIELD_SIZE - 1);
    return Number(id);
}


Number Number::div(const Number& number1, const Number& number2) {
    if (number1.id == FIELD_SIZE - 1 || number2.id == FIELD_SIZE - 1) {
        // return Number::getZero();
        std::cout << "USAGE OF ZERO IN MULTIPLICATION(division) FIELD. TERMINATING" << std::endl;
        exit(-1);
    }
    const unsigned char id = ((FIELD_SIZE - 1) + number1.id - number2.id) % (FIELD_SIZE - 1);
    return Number(id);
}


unsigned char Number::getIdByCode(unsigned char code) {
    if (firstLaunch) {
        firstLaunch = false;
        fillMaps();
    }

    return idByCode[code];
}


unsigned char Number::getCodeById(unsigned char id) {
    if (firstLaunch) {
        firstLaunch = false;
        fillMaps();
    }

    return codeById[id];
}


bool Number::isZero() const {
    return id == (FIELD_SIZE - 1);
}


Number Number::getZero() {
    return Number(FIELD_SIZE - 1);
}


void Number::fillMaps() {
    const unsigned char LAST = (1 << SYMBOL_LENGTH) + (0b11);
    unsigned char code = 0b00000001;
    for(unsigned char id = 0; id < FIELD_SIZE - 1; id++) { // 0 ausgenommen
        codeById[id] = code;
        idByCode[code] = id;
        // prepare next
        code <<= 1;
        if (code >= FIELD_SIZE) {
            code ^= LAST;
        }
    }


    // For addicative group
    codeById[FIELD_SIZE - 1] = 0;
    idByCode[0] = FIELD_SIZE - 1;
}


std::ostream& operator<<(std::ostream& stream, const Number& number) {
    stream << "(a";
    if (number.id == FIELD_SIZE - 1) {
        stream << "#";
    } else {
        stream << ((unsigned int) number.id);
    }
    stream << " = ";

    unsigned char code = number.getCode();
    unsigned char curr = (1 << (SYMBOL_LENGTH - 1));
    for (unsigned char i = SYMBOL_LENGTH; i > 0; i--) {
        if (code / curr >= 1) {
            stream << "1";
        } else {
            stream << "0";
        }

        code %= curr;
        curr >>= 1;
    }
    stream << ")";

    return stream;
}


std::ostream& operator<<(std::ostream& stream, const Polynom& polynom) {
    for (const Number& number : polynom) {
        stream << number << " ";
    }
    return stream;
}
