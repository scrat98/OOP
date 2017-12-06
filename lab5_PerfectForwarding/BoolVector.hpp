#ifndef LAB5_PERFECTFORWARDING_MYVECTOR_HPP
#define LAB5_PERFECTFORWARDING_MYVECTOR_HPP

#include <cstdio>

class BoolVector {
public:
    // make it public, for easiest debugging
    size_t size;
    bool* arr;

    BoolVector() {
        size = 8;
        arr = new bool[size];
    };

    ~BoolVector() {
        delete[] arr;
        std::cout << "Destruct the: " << this << std::endl;
    }

    // copy construction
    BoolVector(const BoolVector& other) {
        size = other.size;
        arr = new bool[size];
        for (size_t i = 0; i < other.size; i++)
            arr[i] = other.arr[i];

        std::cout << "Copy address: " << this << std::endl;
        std::cout << "Copy array address: " << this->arr << std::endl;
    }

    // move construction
    BoolVector(BoolVector&& rvalue) noexcept {
        size = rvalue.size;
        arr = rvalue.arr;
        // we must "steal" array from rvalue, because it calls the destructor
        // and destroyed our array
        rvalue.arr = nullptr;

        std::cout << "Move address: " << this << std::endl;
        std::cout << "Move array address: " << this->arr << std::endl;
    }

    static BoolVector BoolVectorSupplier() {
        BoolVector v;
        std::cout << "Supplier address: " << &v << std::endl;
        std::cout << "Supplier array address: " << v.arr << std::endl;

        return v;
    }

    // operator= for the lvalue. We should make a copy all of our data - deep copy
    BoolVector& operator=(const BoolVector& other) {
        std::cout << "Operator= by lvalue\n";

        if(this == &other) return *this;

        if (other.size > size) {
            delete[] arr;
            arr = new bool[other.size];
        }

        size = other.size;
        for (size_t i = 0; i < other.size; i++)
            arr[i] = other.arr[i];

        return *this;
    }

    // operator= for the rvalue. We should just "steal" rvalue to our data - short copy
    BoolVector& operator=(BoolVector&& rvalue) noexcept {
        std::cout << "Operator= by rvalue\n";

        size = rvalue.size;
        arr = rvalue.arr;
        rvalue.arr = nullptr;

        return *this;
    }
};

#endif //LAB5_PERFECTFORWARDING_MYVECTOR_HPP