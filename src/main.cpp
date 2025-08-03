// benchmark to beat = 1.555468649 (rust)
// Achieved            1.202134    (C++ (This file))

#include <iostream>
#include <cstdint>
#include <thread>
#include <vector> //OHHHH YEAHHH (i had to)
#include <chrono>
using namespace std;

const uint64_t CPUthreadCount = static_cast<uint64_t>(std::thread::hardware_concurrency() * 2);

//const uint64_t CPUthreadCount = 6; // Uncomment to run custom number of threads

const uint64_t Target = 2500;//benchmark value

uint64_t result[Target - 1] = {};

bool Prime(uint64_t num) // c doesnt have a function to check for primes
{
    if (num <= 1) return false;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (uint64_t i = 2; i < num; i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

uint64_t qmod(uint64_t n , uint64_t x , uint64_t b)
{
    uint64_t r = 1;
    uint64_t base = n % b;
    uint64_t exp = x;

    while (exp > 0)
    {
        if ((exp % 2) == 1)
        {
            r = (r * base) % b;
        }
        base = (base*base) % b;
        exp /= 2;
    }
    return r;
}

/*
    let mut r = 1;
    let mut base = n % b;
    let mut exp = x;
    while exp > 0 {
        if exp % 2 == 1 {
            r = (r * base) % b;
        }
        base = (base * base) % b;
        exp /= 2;
    }
    r

*/
uint64_t CycleLength(uint64_t n, uint64_t b)
{

    bool seen[Target] = {};
    uint64_t x = 1;

    while (true)
    {
        uint64_t Qmod = qmod(n,x,b);
        if (seen[Qmod])
        {
            break;
        }
        seen[Qmod] = true;
        x++;
    }
    return x - 1;
}

/* 
    let mut seen = [false; GOAL];
    let mut x = 1;
    loop {
        let qmod = qmod(n, x, b) as usize;
        if seen[qmod] {
            break;
        }
        seen[qmod] = true;
        x += 1;
    }
    x - 1
    */
uint64_t HighestCycleLength(uint64_t x)
{
    if (Prime(x))
    {
        return x - 1;
    }
    else 
    {
        uint64_t m = 0;
        for (uint64_t i = 2; i < x; i++)
        {
            uint64_t b = CycleLength(i,x);
            if (b > m)
            {
                m = b;
            }
        }
        return m;
    }
}
/*
    if is_prime(x) {
        x - 1
    } else {
        let mut m = 1;
        for i in 2..x {
            let b = cyclength(i, x);
            if b > m {
                m = b;
            }
        }
        m
    }
*/

bool worker(uint8_t WorkerNum)
{
    /*
    results_t[i] = highest_cyclength(x);
    if x % 100 == 0 {
        eprintln!("{x}");
    }*/

    for (uint64_t i = WorkerNum; i <= Target; i += CPUthreadCount)
    {
        result[i] = HighestCycleLength(i);

        if ((i % 500) == 0)
        {
            std::cout << i << std::endl;
        }
    }
    std::cout << "Worker Finished: " << WorkerNum << std::endl;
    return true;
}



int main()
{
    //start timer
    auto StartTime = std::chrono::high_resolution_clock::now();

    std::cout << "Thread count: " << CPUthreadCount << std::endl;

    std::vector<std::thread> threads;

    for (uint64_t i = 1; i < CPUthreadCount + 1; i++)
    {
        threads.emplace_back(worker , i);
        std::cout << "Sent out worker:  " << i << std::endl;
    }

    for (auto& thread : threads) { // when a thread finishes
        thread.join();
    }

    //stop time
    auto EndTime = std::chrono::high_resolution_clock::now();
    auto durationChrono = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime);

    std::cout << "Done in: " << durationChrono.count() << "   MicroSeconds using " << CPUthreadCount << "  Threads " << std::endl;
}