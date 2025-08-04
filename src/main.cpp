//          running till base 2500
// benchmark to beat = 1.555468649 (rust)
// Achieved            1.330082    (C++ (This file))

//          running till base 10,000
//  benchmark to beat = 87.57     (rust)
//  Achieved            43.304906 (C++ (This file))

#include <iostream>
#include <cstdint>
#include <thread>
#include <vector> //OHHHH YEAHHH (i had to)
#include <chrono>
#include <atomic>
using namespace std;

const uint32_t CPUthreadCount = static_cast<uint32_t>(std::thread::hardware_concurrency());

//const uint32_t CPUthreadCount = 6; // Uncomment to run custom number of threads

const uint32_t Target = 10000;

std::atomic<uint32_t> result[Target] = {0};

uint32_t seen[Target] = {};
uint32_t Pass = 1;

bool Prime(uint32_t num) {
    if (num <= 1) return false;
    if (num == 2 || num == 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (uint32_t i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

uint32_t qmod(uint32_t n , uint32_t x , uint32_t b)
{
    uint32_t r = 1;
    uint32_t base = n % b;
    uint32_t exp = x;

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

uint32_t CycleLength(uint32_t n, uint32_t b) {
    uint32_t x = 1;
    Pass++; // mark a new round
    while (true) {
        uint32_t q = qmod(n, x, b);
        if (seen[q] == Pass)
            break;
        seen[q] = Pass;
        x++;
    }
    return x - 1;
}


inline uint32_t HighestCycleLength(uint32_t x)
{
    if (Prime(x))
    {
        return x - 1;
    }
    else 
    {
        uint32_t m = 0;
        for (uint32_t i = 2; i < x; i++)
        {
            uint32_t b = CycleLength(i,x);
            if (b > m)
            {
                m = b;
            }
        }
        return m;
    }
}


inline void worker(uint8_t WorkerNum)
{

    std::this_thread::sleep_for(std::chrono::milliseconds(1 * WorkerNum)); // sleeps the thread enough so that race conditions with the first value shouldnt happen
    uint32_t LastViewed = 2;// last element this worker worked on
    
    //std::cout << "result: " << result[Target - 1] << std::endl;

    while (result[Target - 1] == 0) // while the last result doesnt have a value greater than 0
    {
        for (uint32_t i = LastViewed; i < Target; i++)
        {
            uint32_t Zero = 0;
            if (result[i].compare_exchange_strong(Zero,UINT32_MAX))
            {
                result[i] = 1; // this 'cell' is now occupied

                LastViewed = i;

                result[i].store(HighestCycleLength(i) , std::memory_order_relaxed);
                //std::cout << i << ":"<< result[i] << std::endl;
                /*if ((i % 500) == 0)
                {
                    std::cout << "Done till: "<< i << std::endl;
                }*/
                break;
            }
        }
        
    }
    

    //std::cout << "Worker Finished: " << WorkerNum << std::endl;
}



int main()
{
    //start timer
    auto StartTime = std::chrono::high_resolution_clock::now();

    //std::cout << "Thread count: " << CPUthreadCount << std::endl;

    
    

    std::vector<std::thread> threads;

    for (uint32_t i = 1; i < CPUthreadCount + 1; i++)
    {
        threads.emplace_back(worker , i);
        //std::cout << "Sent out worker:  " << i << std::endl;
    }

    for (auto& thread : threads) { // when a thread finishes
        thread.join();
    }

    //stop time
    auto EndTime = std::chrono::high_resolution_clock::now();
    auto durationChrono = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime);

    for (uint32_t i = 0; i<500-1;i++)//print out output
    {
        std::cout << i << ":"<< result[i] << std::endl;
    }
    std::cout << "Done in: " << durationChrono.count() << "   MicroSeconds using " << CPUthreadCount << "  Threads " << std::endl;
}