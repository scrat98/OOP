#pragma once

#include <memory>
#include <cstdint>
#include <map>
#include <string>
#include <core/FactorizationAlgorithm.hpp>
#include <core/Factorizer.hpp>
#include <core/Randomizer.hpp>
#include <utils/CommonFunctions.hpp>

class CheckSystem {
public:
    static uint64_t getNumberFromFactors(std::map<uint64_t, uint64_t> factors);

    static uint64_t generateRandomNDigitsInteger(int digits);

    static bool correctNumberFactorization(std::map<uint64_t, uint64_t> factors, uint64_t n);

    void addAlgorithm(const std::string& name, const AlgorithmSupplier& supplier);

    void deleteAlgorithm(const std::string& name);

    void activateAlgorithm(const std::string& name);

    void disableAlgorithm(const std::string& name);

    void testForNumbers(uint64_t startNumber, uint64_t endNumber, uint64_t step);

    void testForDigits(int startDigits, int endDigits, int cycles);

private:
    std::map<std::string, AlgorithmSupplier> algorithms;
};