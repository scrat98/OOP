#pragma once

#include <core/FactorizationAlgorithm.hpp>
#include <utils/CommonFunctions.hpp>

class BrentAlgorithm : public FactorizationAlgorithm {
public:
    uint64_t getFactor(uint64_t n) {
        if (isPrime(n) || n < 2) return n;
        if (n % 2 == 0) return 2;

        uint64_t x, y, c;
        pseudoRandomFunctionInit(x, y, c, n);
        uint64_t d = 1;

        for (uint64_t i = 1;; i++) {
            d = gcd(std::max(x, y) - std::min(x, y), n); // d = gcd(|x - y|, n)
            if (d != 1 && d != n) break;
            if (integralPowerOf2(i)) y = x;
            x = getPseudoRandomNumber(x, c, n); // x = g(i)
            if (x == y) {
                pseudoRandomFunctionInit(x, y, c, n);
            }
        }

        return d;
    }

private:
    uint64_t getPseudoRandomNumber(uint64_t x, uint64_t c, uint64_t mod) {
        // return (multiplyByModulo(x, x, mod) + c) % mod;

        boost::multiprecision::uint256_t tmp(1);
        tmp = (tmp * x * x) % mod;
        tmp = (tmp + c) % mod;
        return boost::numeric_cast<uint64_t>(tmp);
    }

    void pseudoRandomFunctionInit(uint64_t& x, uint64_t& y, uint64_t& c, const uint64_t& n) {
        x = (Randomizer::getRand<uint64_t>() % (n - 2)) + 2;
        y = x;
        c = (Randomizer::getRand<uint64_t>() % (n - 1)) + 1;
    }
};

const AlgorithmSupplier BrentAlgorithmSupplier = []() {
    return std::make_unique<BrentAlgorithm>();
};