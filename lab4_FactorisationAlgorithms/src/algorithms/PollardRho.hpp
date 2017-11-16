#pragma once

#include <core/FactorizationAlgorithm.hpp>
#include <core/Randomizer.hpp>
#include <utils/CommonFunctions.hpp>

class PollardPhoAlgorithm : public FactorizationAlgorithm {
public:
    uint64_t getFactor(uint64_t n) {
        if (isPrime(n) || n < 2) return n;
        if (n % 2 == 0) return 2;

        uint64_t x, y, c;
        pseudoRandomFunctionInit(x, y, c, n);
        uint64_t d = 1;

        while (d == 1 || d == n) {
            x = getPseudoRandomNumber(x, c, n); // g(i)
            y = getPseudoRandomNumber(getPseudoRandomNumber(y, c, n), c, n); // g(2*i)
            d = gcd(std::max(x, y) - std::min(x, y), n); // d = gcd(|x - y|, n)
            if (x == y) {
                pseudoRandomFunctionInit(x, y, c, n);
            }
        }

        return d;
    }

private:
    uint64_t getPseudoRandomNumber(uint64_t x, uint64_t c, uint64_t mod) {
        // return ((powByModulo(x, 2, mod) + c) % mod);

        boost::multiprecision::uint128_t tmp = 1;
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

const AlgorithmSupplier PollardPhoAlgorithmSupplier = []() {
  return std::make_unique<PollardPhoAlgorithm>();
};
