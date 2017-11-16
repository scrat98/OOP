#include <core/CheckSystem.hpp>

uint64_t CheckSystem::getNumberFromFactors(std::map<uint64_t, uint64_t> factors) {
    if (factors.empty()) return 0;

    uint64_t n = 1;
    for (auto& factor : factors) {
        n *= powByModulo(factor.first, factor.second, UINT64_MAX);
    }

    return n;
}

uint64_t CheckSystem::generateRandomNDigitsInteger(int digits) {
    uint64_t number = 0;

    for (int i = 0; i < digits; i++) {
        int digit = 0;

        if (i == 0) {
            while (1) {
                digit = Randomizer::getRand<int>() % 10;
                if (digit != 0) break;
            }
        } else {
            digit = Randomizer::getRand<int>() % 10;
        }

        number = number * 10 + digit;
    }

    return number;
}

bool CheckSystem::correctNumberFactorization(std::map<uint64_t, uint64_t> factors, uint64_t n) {
    for (auto& factor : factors) {
        if (!isPrime(factor.first)) return false;
    }

    if (getNumberFromFactors(factors) != n) return false;

    return true;
}

void CheckSystem::addAlgorithm(const std::string& name, const AlgorithmSupplier& supplier) {
    algorithms[name] = supplier;
}

void CheckSystem::deleteAlgorithm(const std::string& name) {
    algorithms.erase(name);
}

void CheckSystem::activateAlgorithm(const std::string& name) {

}

void CheckSystem::disableAlgorithm(const std::string& name) {

}

void CheckSystem::testForNumbers(uint64_t startNumber, uint64_t endNumber, uint64_t step = 1) {
    for (uint64_t i = startNumber; i < endNumber; i += step) {

    }
}

void CheckSystem::testForDigits(int startDigits, int endDigits, int cycles) {
    for (int digits = startDigits; digits < endDigits; digits++) {
        for (int i = 0; i < cycles; i++) {
            uint64_t num = CheckSystem::generateRandomNDigitsInteger(digits);
            for (auto& algorithm: algorithms) {
                const auto& algorithmSupplier = algorithm.second;
                Factorizer factorizer(algorithmSupplier);
                auto primeFactors = factorizer.getPrimeFactors(num);
                if (!CheckSystem::correctNumberFactorization(primeFactors, num)) {
                    std::cout << "Number " << num << " was failed in " << algorithm.first << "\n";
                }
            }
        }
    }
}