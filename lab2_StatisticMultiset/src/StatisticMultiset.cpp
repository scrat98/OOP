#include "StatisticMultiset.hpp"

template<class T>
StatisticMultiset<T>::StatisticMultiset() : StatisticMultiset(128)
{}

template<class T>
StatisticMultiset<T>::StatisticMultiset(int cacheSize) : cacheSize(cacheSize), avg(0)
{}

template<class T>
StatisticMultiset<T>::~StatisticMultiset(){}

template<class T>
void StatisticMultiset<T>::addNum(const T& number)
{
    data.insert(number);
    updateAvg(number);
}

template<class T>
void StatisticMultiset<T>::addNum(const std::multiset<T>& numbers)
{
    for (auto& it : numbers) {
        addNum(it);
    }
}

template<class T>
void StatisticMultiset<T>::addNum(const std::vector<T>& numbers)
{
    for (auto& it : numbers) {
        addNum(it);
    }
}

template<class T>
void StatisticMultiset<T>::addNum(const std::list<T>& numbers)
{
    for (auto& it : numbers) {
        addNum(it);
    }
}

template<class T>
void StatisticMultiset<T>::addNum(const StatisticMultiset& a_stat_set)
{
    addNum(a_stat_set.data);
}

template<class T>
void StatisticMultiset<T>::addNum(std::istream& streamContent)
{
    if (!streamContent.good()) return;
    T x;
    while (streamContent >> x) {
        addNum(x);
    }
}

template<class T>
void StatisticMultiset<T>::addNum(const std::string& fileName)
{
    std::fstream file;
    file.open(fileName);
    addNum(file);
}

template<class T>
T StatisticMultiset<T>::getMax() const
{
    return *data.crbegin();
}

template<class T>
T StatisticMultiset<T>::getMin() const
{
    return *data.cbegin();
}

template<class T>
double StatisticMultiset<T>::getAvg() const
{
    return avg;
}

template<class T>
void StatisticMultiset<T>::updateAvg(const T& newNum)
{
    avg = (avg * (data.size() - 1) + newNum) / data.size();
}

template<class T>
long StatisticMultiset<T>::getCountUnder(double threshold)
{
   if (cacheUnder.count(threshold)) {
       return cacheUnder[threshold];
   } else {
       long count = std::distance(data.begin(), data.lower_bound(threshold));
       updateCacheUnder(threshold, count);
       return count;
   }
}

template<class T>
long StatisticMultiset<T>::getCountAbove(double threshold)
{
    if (cacheAbove.count(threshold)) {
        return cacheAbove[threshold];
    } else {
        long count = std::distance(data.upper_bound(threshold), data.end());
        updateCacheAbove(threshold, count);
        return count;
    }
}

template<class T>
void StatisticMultiset<T>::updateCacheUnder(const double& threshold, const long& count)
{
    if (queueUnder.size() > cacheSize) {
        cacheUnder.erase(queueUnder.back());
        queueUnder.pop_back();
    }

    queueUnder.push_back(threshold);
    cacheUnder[threshold] = count;
}

template<class T>
void StatisticMultiset<T>::updateCacheAbove(const double& threshold, const long& count)
{
    if (queueAbove.size() > cacheSize) {
        cacheAbove.erase(queueAbove.back());
        queueAbove.pop_back();
    }

    queueAbove.push_back(threshold);
    cacheAbove[threshold] = count;
}

template class StatisticMultiset<int>;
template class StatisticMultiset<double>;
template class StatisticMultiset<float>;
template class StatisticMultiset<unsigned int>;
template class StatisticMultiset<double>;
template class StatisticMultiset<float>;
template class StatisticMultiset<long long>;
template class StatisticMultiset<long double>;