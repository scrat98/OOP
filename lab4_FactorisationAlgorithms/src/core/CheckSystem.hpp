#pragma once

#include <cstdint>
#include <tuple>
#include <map>
#include <string>
#include <fstream>

class CheckSystem {
public:
    static uint64_t getNumberFromFactors(std::map<uint64_t, uint64_t> factors);

    static uint64_t getRandomNDigitsInteger(int digits);

    static std::string getFactorisationView(std::map<uint64_t, uint64_t> factors);

    static bool isCorrectFactorization(std::map<uint64_t, uint64_t> factors, uint64_t n);

    void addAlgorithm(const std::string& name, const AlgorithmSupplier& supplier);

    void deleteAlgorithm(const std::string& name);

    void setThreadsCount(size_t threadsCount);

    size_t getThreadsCount();

    void testForNumbers(uint64_t startNumber, uint64_t endNumber, uint64_t step, int cycles);

    void testForDigits(int startDigits, int endDigits, uint64_t step, int cycles);

    // TODO timeLimit

    // TODO resume, pause, stop

    // TODO current information in std output

    // TODO graphs with errors

private:
    template<class F, class... Args>
    void test(F&& testFunction, Args&& ... testArgs);

    template<class F, class... Args>
    void testingAlgoritms(F&& testFunction, Args&& ... testArgs);

    void makeGraphs();

    static auto getTestAlgorithmForNumbers();

    static auto getTestAlgorithmForDigits();

    size_t threadsCount = 4;
    std::map<std::string, AlgorithmSupplier> algorithms;
};