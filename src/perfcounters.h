#ifndef DOGECOIN_PERFORMANCE_COUNTERS
#define DOGECOIN_PERFORMANCE_COUNTERS

#include <chrono>
#include <map>
#include <string>

#include "util.h"

class CPerformanceCounters
{
    const std::string category;
    const std::chrono::steady_clock::duration period;
    std::map<std::string, intmax_t> counters;
    std::chrono::steady_clock::time_point period_start;

public:
    CPerformanceCounters(const std::string& cat, std::chrono::steady_clock::duration per) : category(cat), period(per)
    {
        period_start = std::chrono::steady_clock::now();
    }
    void add(const std::string& type, intmax_t n = 1)
    {
        auto t(std::chrono::steady_clock::now());
        auto r(counters.emplace(type, n));
        if (!r.second) r.first->second += n;
        if (t < period_start + period) return;
        auto dt(std::chrono::duration_cast<std::chrono::duration<double>>(t - period_start).count());
        LogPrintf("%s per second:\n", category.c_str());
        for (auto p(counters.begin()), e(counters.end()); p != e; ++p)
            LogPrintf("%s: %f\n", p->first.c_str(), p->second / dt);
        counters.clear();
        period_start = t;
    }
};

#endif
