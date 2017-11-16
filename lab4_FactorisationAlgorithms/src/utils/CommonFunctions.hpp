#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <core/Randomizer.hpp>
#include <exception>

bool isSquare(boost::multiprecision::uint128_t squared);

uint64_t gcd(uint64_t a, uint64_t b);

bool integralPowerOf2(uint64_t n);

uint64_t multiplyByModulo(uint64_t a, uint64_t b, uint64_t m);

uint64_t powByModulo(uint64_t a, uint64_t b, uint64_t m);

bool isPrime(uint64_t n);