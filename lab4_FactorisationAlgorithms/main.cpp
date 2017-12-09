#include <iostream>
#include <core/Factorizer.hpp>
#include <core/CheckSystem.hpp>
#include <algorithms/TrialDivision.hpp>
#include <algorithms/Fermat.hpp>
#include <algorithms/PollardRho.hpp>
#include <algorithms/Brent.hpp>
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
//    uint64_t n;
//    Factorizer fact(PollardPhoAlgorithmSupplier);
//    std::cin >> n;
//    auto primes = fact.getPrimeFactors(n);
//    std::cout << CheckSystem::getFactorisationView(primes) << std::endl;

    CheckSystem sys;
    sys.setThreadsCount(4);
    sys.setTimeLimit(20);
    sys.addAlgorithm("trial", TrialDivisionAlgorithmSupplier);
    sys.addAlgorithm("fermat", FermatAlgorithmSupplier);
    sys.addAlgorithm("pollardRho", PollardPhoAlgorithmSupplier);
    sys.addAlgorithm("pollardBrent", BrentAlgorithmSupplier);

    sys.testForNumbers(1e5, 1e6, 1, 1);

    system("pause");
}