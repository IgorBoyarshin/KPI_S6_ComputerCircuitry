#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <utility>
#include "Number.h"
#include "Util.h"


// <symbol, bit> (from lowest)
typedef std::pair<unsigned int, unsigned int> Distortion;


class Sender {
    public:
        static Polynom getMessageFromSecret(const std::string& secret);
        static Polynom generateBasePolynom(unsigned int symbolsToFix);
        static Polynom div(Polynom pol1, const Polynom& pol2);
        static Polynom distort(Polynom polynom, const std::vector<Distortion>& distortions);
};


#endif
