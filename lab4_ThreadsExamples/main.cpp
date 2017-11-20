#include <iostream>
#include <thread>
#include "ThreadPool.hpp"

void foo() {
    std::cout << "Task 1";
}
int main() {
    std::cout << std::thread::hardware_concurrency() << " concurrent threads are supported." << std::endl;
    ThreadPool threadPool(8);
//    threadPool.addTask([](){std::cout << "Task 1";});
    threadPool.addTask(foo);

    system("pause");

    return 0;
}