#include <chrono>
#include <memory>
#include <vector>
#include <map>
#include <core/FactorizationAlgorithm.hpp>
#include <core/Factorizer.hpp>
#include <core/Randomizer.hpp>
#include <utils/CommonFunctions.hpp>
#include <core/CheckSystem.hpp>
#include <core/ThreadPool.hpp>

uint64_t CheckSystem::getNumberFromFactors(std::map<uint64_t, uint64_t> factors) {
    if (factors.empty()) return 0;

    uint64_t n = 1;
    for (auto& factor : factors) {
        n *= powByModulo(factor.first, factor.second, UINT64_MAX);
    }

    return n;
}

uint64_t CheckSystem::getRandomNDigitsInteger(int digits) {
    uint64_t number = 0;

    for (int i = 0; i < digits; i++) {
        int digit = 0;

        if (i == 0) {
            while (true) {
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

std::string CheckSystem::getFactorisationView(std::map<uint64_t, uint64_t> factors) {
    std::string view;
    for (auto it = factors.begin(); it != factors.end(); it++) {
        view += std::to_string(it->first) + "^" + std::to_string(it->second);
        if (it != --factors.end()) view += " * ";
    }

    return view;
}

bool CheckSystem::isCorrectFactorization(std::map<uint64_t, uint64_t> factors, uint64_t n) {
    for (auto& factor : factors) {
        if (!isPrime(factor.first)) return false;
    }

    return getNumberFromFactors(factors) == n;
}

void CheckSystem::addAlgorithm(const std::string& name, const AlgorithmSupplier& supplier) {
    algorithms[name] = supplier;
}

void CheckSystem::deleteAlgorithm(const std::string& name) {
    algorithms.erase(name);
}

void CheckSystem::setThreadsCount(size_t threadsCount) {
    this->threadsCount = threadsCount;
}

size_t CheckSystem::getThreadsCount() {
    return this->threadsCount;
}

template<class F, class... Args>
void CheckSystem::test(F&& testFunction, Args&& ... testArgs) {
    testingAlgoritms(testFunction, testArgs...);
    makeGraphs();
}

template<class F, class... Args>
void CheckSystem::testingAlgoritms(F&& testFunction, Args&& ... testArgs) {
    ThreadPool threadPool(threadsCount);

    for (auto& algorithm: algorithms) {
        const auto& algorithmName = algorithm.first;
        const auto& algorithmSupplier = algorithm.second;
        threadPool.addTask(testFunction(), algorithmSupplier, algorithmName, testArgs...);
    }

    while (!threadPool.isTasksReady()) {

    }
}

void CheckSystem::makeGraphs() {

}

auto CheckSystem::getTestAlgorithmForNumbers() {
    return [](const AlgorithmSupplier& supplier, const std::string& algorithmName,
              uint64_t startNumber,
              uint64_t endNumber, uint64_t step,
              int cycles) {
        std::ofstream log(algorithmName + ".log");
        std::ofstream cycle(algorithmName + "_cycle.graph");
        std::ofstream avgTime(algorithmName + "_avgTime.graph");
        std::ofstream maxTime(algorithmName + "_maxTime.graph");
        std::string status;

        for (uint64_t num = startNumber; num < endNumber; num += step) {
            Factorizer factorizer(supplier);
            for (int i = 0; i < cycles; i++) {
                status = "OK";

                time_t start = clock();
                auto primeFactors = factorizer.getPrimeFactors(num);
                time_t time = clock() - start;

                if (!CheckSystem::isCorrectFactorization(primeFactors, num))
                    status = "WA";

                log << "Current cycle: " << i << std::endl;
                log << "Current number: " << num << std::endl;
                log << "Result: " << getFactorisationView(primeFactors) << std::endl;
                log << "Time: " << time << std::endl;
                log << "Status: " << status << std::endl;
                log << std::endl;
            }
        }

        log << "Algorithm: " << algorithmName << std::endl;
        log << "All time: " << "?" << std::endl;
        log << "Wrong answers: " << "?" << std::endl;
        log << "Time limited: " << "?" << std::endl;
        log << std::endl;
    };
}

auto CheckSystem::getTestAlgorithmForDigits() {
    return [](const AlgorithmSupplier& supplier, const std::string& algorithmName,
              uint64_t startDigit,
              uint64_t endDigit, uint64_t step,
              int cycles) {

    };
}

void CheckSystem::testForNumbers(uint64_t startNumber, uint64_t endNumber, uint64_t step, int cycles) {
    test(getTestAlgorithmForNumbers, startNumber, endNumber, step, cycles);
}

void CheckSystem::testForDigits(int startDigits, int endDigits, uint64_t step, int cycles) {
    test(getTestAlgorithmForDigits, startDigits, endDigits, step, cycles);
}