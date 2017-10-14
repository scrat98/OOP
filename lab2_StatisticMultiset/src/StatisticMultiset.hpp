//
//  StatisticMultiset.hpp
//  Laba2 OOP
//

#ifndef StatisticMultiset_hpp
#define StatisticMultiset_hpp

#include <set>
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <queue>

template<class T>
class StatisticMultiset
{
public:
    StatisticMultiset();
    StatisticMultiset(int cacheSize);
    ~StatisticMultiset();

    void addNum(const T& number);
    void addNum(const std::multiset<T>& numbers);
    void addNum(const std::vector<T>& numbers);
    void addNum(const std::list<T>& numbers);
    void addNum(const StatisticMultiset& numbers);
    void addNum(std::istream& streamContent);
    void addNum(const std::string& fileName);

    T getMax() const;
    T getMin() const;

    double getAvg() const;

    long getCountUnder(double threshold);
    long getCountAbove(double threshold);

private:
    void updateAvg(const T& newNum);
    void updateCacheUnder(const double& threshold, const long& count);
    void updateCacheAbove(const double& threshold, const long& count);

    std::multiset<T> data;
    const int cacheSize;
    mutable double avg;
    mutable std::map<double, long> cacheUnder;
    mutable std::map<double, long> cacheAbove;
    mutable std::vector<double> queueUnder;
    mutable std::vector<double> queueAbove;
};

#endif /* StatisticMultiset_hpp */