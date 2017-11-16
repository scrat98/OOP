#pragma once

#include <memory>
#include <cstdint>
#include <map>
#include <queue>
#include <core/FactorizationAlgorithm.hpp>

using AlgorithmSupplier = std::function<std::unique_ptr<FactorizationAlgorithm>()>;

class Factorizer {
public:
  explicit Factorizer() = delete;
  explicit Factorizer(const AlgorithmSupplier& supplier) : algorithm(supplier()){}

  std::map<uint64_t, uint64_t> getPrimeFactors(uint64_t n) {
        std::map<uint64_t, uint64_t> prime_factors;
        std::queue<uint64_t> factors;
        factors.push(n);

        while (!factors.empty()) {
            uint64_t p = factors.front();
            factors.pop();

            uint64_t factor = algorithm->getFactor(p);
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