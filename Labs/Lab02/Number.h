#ifndef NUMBER_H
#define NUMBER_H

#include "constants.h"
#include <map>
#include <ostream>
#include <vector>
#include <iostream>


struct Number {
    public:
        unsigned char id; // index number

        Number();
        Number(unsigned char id);

        Number operator+(const Number& number) const;
        Number operator*(const Number& number) const;
        Number operator/(const Number& number) const;
        Number operator^(unsigned int power) const;

        unsigned char getCode() const;
        bool isZero() const;

    public:
        static Number add(const Number& number1, const Number& number2);
        static Number mul(const Number& number1, const Number& number2);
        static Number div(const Number& number1, const Number& number2);
        static Number pow(const Number& number1, unsigned int power);

        static unsigned char getIdByCode(unsigned char code);
        static unsigned char getCodeById(unsigned char id);

        static Number getZero();

    private:
        static void fillMaps();

    private:
        static bool firstLaunch;
        static unsigned char idByCode[FIELD_SIZE];
        static unsigned char codeById[FIELD_SIZE];
};


typedef std::vector<Number> Polynom;


std::ostream& operator<<(std::ostream& stream, const Number& number);
std::ostream& operator<<(std::ostream& stream, const Polynom& polynom);


#endif
