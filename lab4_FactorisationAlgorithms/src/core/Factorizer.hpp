#pragma once

#include <memory>
#include <cstdint>
#include <map>
#include <queue>
#include <core/FactorizationAlgorithm.hpp>

//template<class Algorithm, class = class std::enable_if<std::is_base_of<FactorizationAlgorithm, Algorithm>::value, Algorithm>::type>
class Factorizer {
public:
    Factorizer(FactorizationAlgorithm* algorithm) : algorithm(std::unique_ptr<FactorizationAlgorithm>(algorithm)) {};

    std::map<uint64_t, uint64_t> getPrimeFactors(uint64_t n) {
        std::map<uint64_t, uint64_t> prime_factors;
        std::queue<uint64_t> factors;
        factors.push(n);

        while (!factors.empty()) {
            uint64_t p = factors.front();
            factors.pop();

            uint64_t factor = algorithm.get()->getFactor(p);
            if (factor == p) {
                prime_factors[factor]++;
            } else {
                factors.push(factor);
                factors.push(p / factor);
            }
        }

        return prime_factors;
    }

private:
    std::unique_ptr<FactorizationAlgorithm> algorithm;
};