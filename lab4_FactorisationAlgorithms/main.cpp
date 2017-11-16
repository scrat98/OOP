#include <iostream>
#include <core/Factorizer.hpp>
#include <core/CheckSystem.hpp>
#include <algorithms/TrialDivision.hpp>
#include <algorithms/Fermat.hpp>
#include <algorithms/PollardRho.hpp>
#include <algorithms/Brent.hpp>

int main(int argc, char** argv) {
    CheckSystem sys;
    sys.addAlgorithm("trial", TrialDivisionAlgorithmSupplier);
    sys.addAlgorithm("fermat", FermatAlgorithmSupplier);
    sys.addAlgorithm("pollardRho", PollardPhoAlgorithmSupplier);
    sys.addAlgorithm("pollardBrent", BrentAlgorithmSupplier);
    sys.testForDigits(1, 19, 1);

    system("pause");
}