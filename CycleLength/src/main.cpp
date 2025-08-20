#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <functional>
#include <numeric>


std::vector<uint32_t> results;     // store results for Numbers 1..Amt

std::vector<uint32_t> PrimeFactors(int n)
{
    std::vector<uint32_t> factors; //temp until Im smarter with memory allocations

    uint32_t d = 2;
    while ((d*d) <= n)
    {
        while ((n % d) == 0)
        {
            factors.push_back(d);
            n /= d;
        }
        d++;
    }

    if (n > 1)
    {
        factors.push_back(n);
    }
    return factors;
}

uint32_t CarmichaelFunction(uint32_t n)
{
    if (n == 1) return 1;

    std::vector<std::pair<uint32_t, uint32_t>> factors; // store (prime, exponent)
    uint32_t Temp = n;
    uint32_t d = 2;

    while ((d*d) <= Temp)
    {
        uint32_t Count = 0;
        while (Temp % d == 0)
        {
            Temp /= d;
            Count ++;
        }
        if (Count > 0)
        {
            factors.push_back(std::make_pair(d, Count));
        }
        d++;

    }
    if (Temp > 1)
    {
        factors.push_back(std::make_pair(Temp, 1));
    }
    

    std::vector<uint32_t> lambdas;

    for (const auto& pk : factors) {
        uint32_t p = pk.first;
        uint32_t k = pk.second;
        if (p == 2) {
            if (k == 1) {
                lambdas.push_back(1);
            } else if (k == 2) {
                lambdas.push_back(2);
            } else {
                lambdas.push_back(1u << (k - 2)); // 2**(k-2)
            }
        } else {
            uint32_t val = (p - 1);
            for (uint32_t i = 1; i < k; ++i) {
                val *= p;
            }
            lambdas.push_back(val);
        }
    }

    uint32_t result = lambdas[0];
    for (size_t i = 1; i < lambdas.size(); i++) {
        result = std::lcm(result, lambdas[i]);
    }
    return result;

}


void DispatchThreads(uint32_t nThreads, uint32_t Amt) {
    std::atomic<uint32_t> next{1};            // atomic counter = work queue

    auto worker = [&]() {
        while (true) {
            uint32_t Num = next.fetch_add(1);
            if (Num > Amt) break;

            int result = CarmichaelFunction(Num);
            results[Num] = result; // safe: each thread writes its own index
        }
    };

    // Launch threads
    std::vector<std::thread> pool;
    for (uint32_t i = 0; i < nThreads; i++) {
        pool.emplace_back(worker);
    }

    // Wait for all threads
    for (auto& t : pool) {
        t.join();
    }


}

int main()
{
    const int ThreadCount = std::thread::hardware_concurrency();
    const int Amt = 2500;

    auto StartTime = std::chrono::high_resolution_clock::now(); //start timer

    results.resize(Amt + 1);     // store results for Numbers 1..Amt
    DispatchThreads(ThreadCount, Amt);

    //stop time
    auto EndTime = std::chrono::high_resolution_clock::now();
    auto durationChrono = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime);

    // Print results
    for (uint32_t i = 1; i <= Amt; i++) {
        std::cout << i << ":"<< results[i] << std::endl;
    }
    std::cout << "Done in: " << durationChrono.count() << "   MicroSeconds using " << ThreadCount << "  Threads " << std::endl;
}