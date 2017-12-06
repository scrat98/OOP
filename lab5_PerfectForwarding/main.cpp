#include <iostream>
#include "BoolVector.hpp"

// See the lessons 
// 1. https://www.youtube.com/watch?v=UTUdhjzws5g
// 2. https://www.youtube.com/watch?v=IOkgBrXCtfo
// 3. https://www.youtube.com/watch?v=0xcCNnWEMgs

// https://habrahabr.ru/post/226229/
// https://habrahabr.ru/post/242639/
// https://habrahabr.ru/post/157961/

void foo(BoolVector v) {
    std::cout << "Vector foo_value address: " << &v << std::endl;
    std::cout << "Vector foo_value array address: " << v.arr << std::endl;
}

void foo_ref(BoolVector& v) {
    std::cout << "Vector foo_ref address: " << &v << std::endl;
    std::cout << "Vector foo_ref array address: " << v.arr << std::endl;
}

template<class T>
void overloaded(const T& arg) { std::cout << "by lvalue\n"; }

template<class T>
void overloaded(T&& arg) { std::cout << "by rvalue\n"; }

// T&& is Universal Reference: rvalue, lvalue, const, non-const, etc...
// Conditions:
// 1. T is template type
// 2. type deduction (reference collapsing) happens to T.
// - T is a function template type, not class template type
template<class T>
void forwarding(T&& arg) {
    std::cout << "via std::forward: ";
    overloaded(std::forward<T>(arg));
    std::cout << "via std::move: ";
    overloaded(std::move(arg));
    std::cout << "by simple passing: ";
    overloaded(arg);
}

// forward implementation
//template<class T>
//T&& forward(typename std::remove_reference<T>::type& t) noexcept {
//    return static_cast<T&&>(t);
//}

// make_pair implementation
//template<typename T, typename U>
//std::pair<T, U> make_pair_wrapper(T&& t, U&& u) {
//    return std::make_pair(std::forward<T>(t),
//                          std::forward<U>(u));
//}

// unique_ptr implementation
//template<typename T, typename... Args>
//unique_ptr <T> make_unique(Args&& ... args) {
//    return unique_ptr<T>(new T(std::forward<Args>(args)...));
//}
// All STL classes support perfect forwarding ( > C++11)

int main() {
    // call move constructor, because we return BoolVectorSupplier by value.
    // And now BoolVector "v" is binary copy of the BoolVectorSupplier.
    // And temporary BoolVectorSupplier was destroyed

    BoolVector v = BoolVector::BoolVectorSupplier();
    std::cout << "Vector_main address: " << &v << std::endl;
    std::cout << "Vector_main array address: " << v.arr << std::endl;

    foo(v); // Call copy constructor
    foo_ref(v); // Call no constructor

    foo(BoolVector::BoolVectorSupplier()); // Call move constructor
    foo(std::move(v)); // Call move constructor and BoolVector "v" destroyed here

    // now, we want to avoid the overloaded function - foo and foo_ref and use perfect forwarding
    // Reference collapsing rule C++11
    // 1. T& &   => T&
    // 2. T& &&  => T&
    // 3. T&& &  => T&
    // 4. T&& && => T&&

    std::cout << "initial caller passes rvalue:\n";
    forwarding(5);
    std::cout << "initial caller passes lvalue:\n";
    int x = 5;
    forwarding(x);

    system("pause");
    return 0;
}