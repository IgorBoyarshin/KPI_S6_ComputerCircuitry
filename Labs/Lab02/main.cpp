#include <iostream>
// #include <cstdlib>
// #include <time.h>
#include "Number.h"
#include "Sender.h"
#include "Util.h"
#include "constants.h"
#include "Receiver.h"

// #define IS_SENDER_NOT_RECEIVER

int main() {
    std::cout << "--------------------------------------------------" << std::endl;

    const std::string secret("Love1My1Anka111");
    std::cout << "Secret: "
        << secret
        << std::endl;

    Polynom message = Sender::getMessageFromSecret(secret);
    {
        const Polynom generatingPolynom = Sender::generateBasePolynom(CORRECTING_MISTAKES);
        const Polynom controlSymbols = Sender::div(message, generatingPolynom);
        for (unsigned int i = 0; i < CONTROL_SYMBOLS; i++) {
            message[i] = controlSymbols[i];
        }
    }
    // const unsigned int messageLength = message.size();
    std::cout << "Complete Message:  "
        << message
        << " === "
        << Util::decode(message)
        << std::endl;

    // const std::vector<Distortion> distortions = {{0, 2}};
    // std::vector<Distortion> distortions;
    // {
    //     srand (time(NULL));
    //     for (int i = 0; i < rand() % 3; i++) {
    //         const unsigned int index = rand() % message.size();
    //         const unsigned int bit = rand() % SYMBOL_LENGTH;
    //
    //         distortions.push_back({index, bit});
    //     }
    // }

    // Distort
    // <place, bit>. 2 distortions tops
    const std::vector<Distortion> distortions = {{5, 0}, {6, 2}};
    std::cout << "Applying distortions: ";
    for (const auto& distortion : distortions) {
        std::cout
            << "at "
            << Number(distortion.first)
            << ": "
            << Number(distortion.second)
            << ", ";
    }
    std::cout << std::endl;

    const Polynom distortedMessage = Sender::distort(message, distortions);
    std::cout << "Distorted message: "
        << distortedMessage
        << " === "
        << Util::decode(distortedMessage)
        << std::endl;

    std::cout << "Sending:  "
        << Util::decode(distortedMessage)
        << std::endl;

    std::cout << "--------------------------------------------------" << std::endl;

    const std::string received = Util::decode(distortedMessage);
    std::cout << "Received: "
        << received
        << std::endl;
    Receiver receiver(Util::encode(received));

    const auto syndromes = receiver.getSyndromes();
    // std::cout << "Syndromes: "
    //         << syndromes
    //         << std::endl;

    if (receiver.isMessageCorrect(syndromes)) {
        std::cout << "Message is undistorted."
            << std::endl
            << "Received secret: "
            << received.substr(CONTROL_SYMBOLS, message.size() - CONTROL_SYMBOLS)
            << std::endl;
        return 0;
    }

    const auto sigmas = receiver.getSigmas(syndromes);
    // std::cout << "Sigmas: " << sigmas.first << " " << sigmas.second << std::endl;

    const auto betas = receiver.getBetas(sigmas);
    // std::cout << "Betas: " << betas.first << " " << betas.second << std::endl;

    const auto zeus = receiver.getZeus(betas, {syndromes[0], syndromes[1]});
    // std::cout << "Zeus: " << zeus.first << " " << zeus.second << std::endl;

    if (sigmas.second.isZero()) {
        std::cout << "There is ONE distortion at "
            << betas.first << ": " << zeus.first
            << std::endl;
    } else {
        std::cout << "There are TWO distortion at "
            << betas.first << ": " << zeus.first
            << " AND "
            << betas.second << ": " << zeus.second
            << std::endl;
    }

    // Fix distortions
    const Polynom fixedReceived = receiver.fixDistortions(
            {{betas.first, zeus.first},
            {betas.second, zeus.second}});
    std::cout << "Fixed message: "
        << Util::decode(fixedReceived)
        << std::endl;

    std::cout << "Received secret: "
        << Util::decode(receiver.stripFromControl(fixedReceived))
        << std::endl;

    return 0;
}
