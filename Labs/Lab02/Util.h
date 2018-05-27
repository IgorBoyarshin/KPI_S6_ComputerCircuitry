#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include "constants.h"
#include "Number.h"


class Util {
    public:
        static Polynom encode(const std::string& message);
        static std::string decode(const Polynom& symbols);

    private:
        static char getChar(const Number& number);
        static Number getNumber(char c);
};


#endif
