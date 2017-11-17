#include <utils/CommonFunctions.hpp>

void clearScreen() {
#if defined(WINDOWS) || defined(WIN32) || defined(WIN64) || defined(MSDOS)
    std::system("cls");
#else
    std::system("clear");
#endif
}

bool isSquare(boost::multiprecision::uint128_t squared) {
    boost::multiprecision::uint128_t sqrt_val = boost::multiprecision::sqrt(squared);
    boost::multiprecision::uint128_t sqr_val = sqrt_val * sqrt_val;

    return sqr_val == squared;
}

uint64_t gcd(uint64_t a, uint64_t b) {
    uint64_t remainder;
    while (b != 0) {
        remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

bool integralPowerOf2(uint64_t n) {
    return !(n & (n - 1));
}

uint64_t multiplyByModulo(uint64_t a, uint64_t b, uint64_t m) {
    if (a == 0 || b == 0) return 0;
    if (b == 1) return a;
    if (a == 1) return b;
    if (b % 2 == 0) {
        uint64_t t = multiplyByModulo(a, b / 2, m);
        return (2 * t) % m;
    }

    return (multiplyByModulo(a, b - 1, m) + a) % m;
}

uint64_t powByModulo(uint64_t a, uint64_t b, uint64_t m) {
    if (a == 0 && b == 0) throw std::invalid_argument("0^0 indefined");
    if (b == 0) return 1;
    if (b == 1) return a;
    if (b % 2 == 0) {
        uint64_t t = powByModulo(a, b / 2, m);
        return multiplyByModulo(t, t, m) % m;
    }

    return (multiplyByModulo(powByModulo(a, b - 1, m), a, m)) % m;
}

bool isPrime(uint64_t n) {
    if (n <= 2) return true;

    for (int i = 0; i < 100; i++) {
        uint64_t a = (Randomizer::getRand<uint64_t>() % (n - 2)) + 2;
        if (gcd(a, n) != 1)
            return false;
        if (powByModulo(a, n - 1, n) != 1)
            return false;
    }

    return true;
}

bool isPrimeTrial(uint64_t n) {
    if (n <= 2) return true;

    for (uint64_t i = 2; i <= std::ceil(std::sqrt(n)); i++) {
        if (n % i == 0)
            return false;
    }

    return true;
}