#include "Sender.h"


Polynom Sender::getMessageFromSecret(const std::string& secret) {
    const Polynom encodedSecret = Util::encode(secret);
    const unsigned int messageSize = CONTROL_SYMBOLS + secret.size();
    Polynom message(messageSize);
    for (unsigned int i = 0; i < messageSize; i++) {
        message[i] = (i < CONTROL_SYMBOLS ?
                Number::getZero() :
                encodedSecret[i - CONTROL_SYMBOLS]);
    }

    return message;
}


// v[i] === i-th from lowest
Polynom Sender::generateBasePolynom(unsigned int symbolsToFix) {
    const unsigned int SIZE = symbolsToFix * 2 + 1;
    Polynom polynom(SIZE, Number::getZero()); // identity in addicative group

    // <power of X, number>
    const auto getProduct = [SIZE](unsigned int counter){
        unsigned int amountOfOnes = 0;
        Number curr(0); // identity in multiplication group
        // i+1 === alpha = [1..2*H] = [1..SIZE-1]
        for (unsigned int i = 0; i < SIZE - 1; i++) {
            if (counter % 2 == 1) {
                amountOfOnes++;
            } else {
                curr = curr * Number(i + 1);
            }
            counter >>= 1;
        }

        return std::pair<unsigned int, Number>(amountOfOnes, curr);
    };

    // i-th bit(from the right) in this variable corresponds to the presence
    // of X in i-th parentheses.
    for (unsigned int counter = 0; counter < (unsigned int)(1 << (SIZE - 1)); counter++) {
        const auto p = getProduct(counter);
        polynom[p.first] = polynom[p.first] + p.second;
    }

    return polynom;
}

Polynom Sender::div(Polynom pol1, const Polynom& pol2) {
    const unsigned int size2 = pol2.size() - 1;
    for (unsigned int size1 = pol1.size() - 1; size1 >= size2; size1--) {
        const unsigned int currX = size1 - size2;
        // pol2[size2] couldn't be zero beause it's the power of pol2
        const Number currNumber = pol1[size1] / pol2[size2];

        // By performing a single step of division we eliminate the highest term
        pol1.erase(pol1.begin() + size1);

        // Iterate over all terms(Xs) in pol2 except for the highest(already taken care of)
        for (int i = size2 - 1; i >= 0; i--) {
            // const Number& pol2Number = pol2[i];
            // if (pol2Number.isZero()) {
            //     continue;
            // }
            // if pol2[i] is zero => result of mul is zero => pol1[..} won't change
            pol1[currX + i] = pol1[currX + i] + (currNumber * pol2[i]);
        }
    }

    return pol1;
}


Polynom Sender::distort(Polynom polynom, const std::vector<Distortion>& distortions) {
    for (const Distortion& distortion : distortions) {
        polynom[distortion.first] = polynom[distortion.first] +
            Number(Number::getIdByCode(1 << distortion.second));
    }

    return polynom;
}
