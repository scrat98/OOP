#include "StatisticMultiset.hpp"
#include <iostream>

int main()
{
    std::cout << "Lab 02" << std::endl;

    StatisticMultiset<int> ms1;
    ms1.addNum(89);
    ms1.addNum(54);
    ms1.addNum(54);
    ms1.addNum(24);

    StatisticMultiset<int> ms2;
    std::vector<int> somedata = { 10, 40, 6, 87 };
    ms2.addNum(somedata);

    StatisticMultiset<int> ms3;
    ms3.addNum(ms1);
    ms3.addNum(ms2);

    std::cout << " Min: " << ms3.getMin()
        << " Avg: " << ms3.getAvg()
        << " Max: " << ms3.getMax()
        << std::endl;

    system("pause");
    return 0;
}