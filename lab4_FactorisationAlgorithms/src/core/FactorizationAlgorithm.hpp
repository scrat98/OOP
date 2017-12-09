#pragma once

#include <cstdint>
#include <functional>

class FactorizationAlgorithm {
public:
    virtual uint64_t getFactor(uint64_t n) = 0;
};

using AlgorithmSupplier = std::function<std::unique_ptr<FactorizationAlgorithm>()>;