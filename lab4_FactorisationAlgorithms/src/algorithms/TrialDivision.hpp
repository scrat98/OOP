#pragma once

#include <core/FactorizationAlgorithm.hpp>
#include <utils/CommonFunctions.hpp>

class TrialDivisionAlgorithm : public FactorizationAlgorithm {
public:
    uint64_t getFactor(uint64_t n) override {
        if (isPrime(n) || n < 2) return n;

        for (uint64_t i = 2; i <= std::ceil(std::sqrt(n)); i++) {
            if (n % i == 0) return i;
        }

        return n;
    }
};

const AlgorithmSupplier TrialDivisionAlgorithmSupplier = []() {
    return std::make_unique<TrialDivisionAlgorithm>();
};
