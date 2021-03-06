#pragma once

#include <core/FactorizationAlgorithm.hpp>
#include <utils/CommonFunctions.hpp>
#include <boost/multiprecision/cpp_int.hpp>

class FermatAlgorithm : public FactorizationAlgorithm {
public:
    uint64_t getFactor(uint64_t n) override {
        if (isPrime(n) || n < 2) return n;
        if (n % 2 == 0) return 2;

        uint64_t x = static_cast<uint64_t>(std::ceil(std::sqrt(n)));
        boost::multiprecision::uint128_t ySquared(1);
        ySquared = ySquared * x * x - n;
        for (; x <= n; x++) {
            if (isSquare(ySquared)) {
                uint64_t y = boost::numeric_cast<uint64_t>(boost::multiprecision::sqrt(ySquared));
                uint64_t s = (x - y);
                uint64_t t = (x + y);

                if (s != 1 && t != n) {
                    return s;
                }
            }
            ySquared += 2 * boost::numeric_cast<boost::multiprecision::uint128_t>(x) + 1;
        }

        return n;
    }
};

const AlgorithmSupplier FermatAlgorithmSupplier = []() {
    return std::make_unique<FermatAlgorithm>();
};