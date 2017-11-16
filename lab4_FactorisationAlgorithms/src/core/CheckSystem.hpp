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

    template<class Algorithm, class = class std::enable_if<std::is_base_of<FactorizationAlgorithm, Algorithm>::value, Algorithm>::type>
    void addAlgorithm(const std::string& name) {
        Algorithm* algorithm = new Algorithm();
        algorithms[name] = std::unique_ptr<FactorizationAlgorithm>(algorithm);
    }

    void deleteAlgorithm(const std::string& name);

    void activateAlgorithm(const std::string& name);

    void disableAlgorithm(const std::string& name);

    void testForNumbers(uint64_t startNumber, uint64_t endNumber, uint64_t step);

    void testForDigits(int startDigits, int endDigits, int cycles);

private:
    std::map<std::string, std::unique_ptr<FactorizationAlgorithm>> algorithms;
};