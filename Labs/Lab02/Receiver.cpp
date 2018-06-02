#include "Receiver.h"


Receiver::Receiver(Polynom message) : message(message), messageSize(message.size()) {}


std::vector<Number> Receiver::getSyndromes() const {
    std::vector<Number> syndromes(CONTROL_SYMBOLS);

    const Polynom& _message = message;
    const unsigned int _messageSize = messageSize;
    const auto getSyndrome = [_messageSize, &_message] (const Number arg) {
        Number syndrome = Number::getZero(); // identity in additive field
        Number poweredArg = Number(0); // identity in multiplicative field
        for (unsigned int i = 0; i < _messageSize; i++) {
            syndrome = syndrome + _message[i] * poweredArg;
            poweredArg = poweredArg * arg;
        }

        return syndrome;
    };

    for (unsigned int index = 1; index <= CONTROL_SYMBOLS; index++) {
        syndromes[index - 1] = getSyndrome(Number(index));
    }

    return syndromes;
}


bool Receiver::isMessageCorrect(const std::vector<Number>& syndromes) const {
    for (const Number& number : syndromes) {
        if (!number.isZero()) {
            return false;
        }
    }

    return true;
}


bool Receiver::areThereTwoDistortions(const std::vector<Number>& syndromes) const {
    return (!det(
            {syndromes[1], syndromes[2]},
            {syndromes[0], syndromes[1]}
            ).isZero());
}


std::pair<Number, Number> Receiver::getSigmas(const std::vector<Number>& syndromes) const {
    if (areThereTwoDistortions(syndromes)) {
        // first and second columns are flipped because sigmas' positions are flipped
        return solve(
                {syndromes[1], syndromes[2]},
                {syndromes[0], syndromes[1]},
                {syndromes[2], syndromes[3]});
    } else {
        return {syndromes[1] / syndromes[0], Number::getZero()};
    }
}


std::pair<Number, Number> Receiver::getBetas(const std::pair<Number, Number>& sigmas) const {
    if (sigmas.second.isZero()) {
        return {sigmas.first, sigmas.second};
    }

    for (unsigned int i = 0; i < FIELD_SIZE - 1; i++) {
        const Number beta1 = Number(i);
        const Number beta2 = sigmas.second / beta1;
        if ((beta1 + beta2).id == sigmas.first.id) {
            return {beta1, beta2};
        }
    }

    std::cout
        << "FAILED TO DERIVE BETAS. PROBABLY AN INVALID MESSAGE WAS RECEIVED. Terminating."
        << std::endl;
    exit(-1);

    // The following piece of code could work as well, but it needs to be fixed first
    //
    // std::pair<Number, Number> betas;
    // bool found1 = false;
    // bool found2 = false;
    // Number arg = Number(0); // identity in multiplicative group
    // const auto calcSyndrome = [&sigmas](const Number& arg) {
    //     return (Number(0) + sigmas.first * arg + sigmas.second * (arg^2));
    // };
    //
    // for (unsigned int i = 0; i < messageSize; i++) {
    //     std::cout << calcSyndrome(arg) << std::endl;
    //     if (calcSyndrome(arg).isZero()) {
    //         if (!found1) {
    //             betas.first = Number(0) / arg;
    //             found1 = true;
    //         } else if (!found2) {
    //             betas.second = Number(0) / arg;
    //             found2 = true;
    //         } else {
    //             std::cout << "FOUND MORE THAN 2 DISTORTIONS. Ignoring" << std::endl;
    //         }
    //     }
    //
    //     arg = arg * Number(1);
    // }
    //
    // return betas;
}


std::pair<Number, Number> Receiver::getZeus(
        const std::pair<Number, Number>& betas,
        const std::pair<Number, Number> syndromes) const {
    if (!betas.second.isZero()) {
        return solve(
                {betas.first, betas.first^2},
                {betas.second, betas.second^2},
                {syndromes.first, syndromes.second});
    } else {
        return {syndromes.first / betas.first, Number::getZero()};
    }
}


Polynom Receiver::fixDistortions(std::vector<std::pair<Number, Number>> distortions) const {
    Polynom fixedPolynom = message;
    for (const auto& distortion : distortions) {
        if (!distortion.first.isZero()) {
            fixedPolynom[distortion.first.id] = fixedPolynom[distortion.first.id] + distortion.second;
        }
    }

    return fixedPolynom;
}


Polynom Receiver::stripFromControl(const Polynom& fixedMessage) const {
    Polynom stripped;
    for (unsigned int i = CONTROL_SYMBOLS; i < fixedMessage.size(); i++) {
        stripped.push_back(fixedMessage[i]);
    }

    return stripped;
}


Number Receiver::det(
        std::pair<Number, Number> col1,
        std::pair<Number, Number> col2) {
    return (col1.first * col2.second + col1.second * col2.first);
}


std::pair<Number, Number> Receiver::solve(
        std::pair<Number, Number> col1,
        std::pair<Number, Number> col2,
        std::pair<Number, Number> col3) {
    const Number d = det(col1, col2);
    if (d.isZero()) {
        std::cout << "DET IS ZERO. Terminating" << std::endl;
        exit(-1);
    }

    return {det(col3, col2) / d, det(col1, col3) / d};
}
