#ifndef RECEIVER_H
#define RECEIVER_H

#include <vector>
#include <utility>
#include "constants.h"
#include "Number.h"


class Receiver {
    public:
        Receiver(Polynom message);

        std::vector<Number> getSyndromes() const;
        bool isMessageCorrect(const std::vector<Number>& syndromes) const;
        bool areThereTwoDistortions(const std::vector<Number>& syndromes) const;
        std::pair<Number, Number> getSigmas(const std::vector<Number>& syndromes) const;
        std::pair<Number, Number> getBetas(const std::pair<Number, Number>& sigmas) const;
        std::pair<Number, Number> getZeus(
                const std::pair<Number, Number>& betas,
                const std::pair<Number, Number> syndromes) const;
        Polynom fixDistortions(std::vector<std::pair<Number, Number>> distortions) const;
        Polynom stripFromControl(const Polynom& fixedMessage) const;


    private:
        const Polynom message;
        const unsigned int messageSize;

    private:
        static Number det(
                std::pair<Number, Number> col1,
                std::pair<Number, Number> col2);
        static std::pair<Number, Number> solve(
                std::pair<Number, Number> col1,
                std::pair<Number, Number> col2,
                std::pair<Number, Number> col3);
};


#endif
