//          runing till base 250
//  benchmark to beat = ?? (rust)
// Achieved             0.101193

//          runing till base 500
//  benchmark to beat = ?? (rust)
// Achieved             0.101254

//          running till base 1000
//  benchmark to beat = ?? (rust)
//  Achieved            0.205093

//          running till base 2500
// benchmark to beat = 1.555468649 (rust)
// Achieved            1.330082    (C++ (This file))

//          running till base 10,000
//  benchmark to beat = 87.57     (rust)
//  Achieved            43.304906 (C++ (This file))

//          running till base 20,000
//  benchmark to beat = ??          (rust)
//  Achieved            262.182384  (C++ (This file))

#include <iostream>
#include <cstdint>
#include <thread>
#include <vector> //Direction and magnitude OH YEAH
#include <chrono>
#include <atomic>
using namespace std;

//const uint16_t CPUthreadCount = static_cast<uint16_t>(std::thread::hardware_concurrency());

const uint16_t CPUthreadCount = 6; // Uncomment to run custom number of threads

const uint16_t Target = 10000; // max is 65,535

const bool NoScreen = false; // set to true for benchmarking. the screen is cosmetic only

std::atomic<uint16_t> result[Target] = {0};

uint16_t LastElement = 0;

uint16_t seen[Target] = {};
uint16_t Pass = 1;

bool Prime(uint16_t num) {
    if (num <= 1) return false;
    if (num == 2 || num == 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (uint16_t i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

/*uint16_t qmod(uint16_t n , uint16_t x , uint16_t b)
{
    uint16_t r = 1;
    uint16_t base = n % b;
    uint16_t exp = x;

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
}*/

uint16_t CycleLength(uint16_t n, uint16_t b) {
    bool Seen[Target] = {false};
    uint16_t Index = 1;
    uint16_t Term = n;
    Seen[n] = true;
    while (true)
    {
        Term = (Term * n) % b;
        Index ++;
        if (Seen[Term])
        {
            break;
        }
        Seen[Term] = true;
    }
    return Index - 1;
}

inline uint16_t HighestCycleLength(uint16_t x)
{
    if (Prime(x))
    {
        return x - 1;
    }else
    {
        uint16_t hclX = 1;
        for (uint16_t n = 2; n < x; n++)
        {
            const uint16_t baseB = CycleLength(n,x);
            if (baseB > hclX)
            {
                hclX = baseB;
            }
        }
        return hclX;
    }
}


/*inline uint16_t HighestCycleLength(uint16_t x)
{
    if (Prime(x))
    {
        return x - 1;
    }
    else 
    {
        uint16_t m = 0;
        for (uint16_t i = 2; i < x; i++)
        {
            uint16_t b = CycleLength(i,x);
            if (b > m)
            {
                m = b;
            }
        }
        return m;
    }
}
*/

inline void worker(uint8_t WorkerNum)
{

    //std::this_thread::sleep_for(std::chrono::milliseconds(1 * WorkerNum)); // sleeps the thread enough so that race conditions with the first value shouldnt happen
    uint16_t LastViewed = WorkerNum - 1;// last element this worker worked on
    
    //std::cout << "result: " << result[Target - 1] << std::endl;

    while (result[Target - 1] == 0) // while the last result doesnt have a value greater than 0
    {
        for (uint16_t i = LastViewed; i < Target; i++)
        {
            uint16_t Zero = 0;
            if (result[i].compare_exchange_strong(Zero,UINT16_MAX))
            {
                result[i] = 1; // this 'cell' is now occupied

                LastViewed = i;

                result[i].store(HighestCycleLength(i) , std::memory_order_relaxed);
                //std::cout << i << ":"<< result[i] << std::endl;
                if ((i % 50) == 0)
                {
                    //std::cout << "Done So far: "<< i << std::endl;
                    LastElement = i;
                }
                break;
            }
        }
        
    }
    

    //std::cout << "Worker Finished: " << WorkerNum << std::endl;
}

void Screen()
{
    if (NoScreen) return;
    while(result[Target - 1] == 0)
    {
        int clear = system("clear");
        std::cout << "Last known Base computed: " << LastElement << std::endl;
        std::cout << "Using: " << CPUthreadCount << " Threads, with a Target of: " << Target << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


int main()
{
    //start timer
    auto StartTime = std::chrono::high_resolution_clock::now();

    //std::cout << "Thread count: " << CPUthreadCount << std::endl;

    
    

    std::vector<std::thread> threads;
    std::thread DisplayThread(Screen);
    for (uint16_t i = 1; i < CPUthreadCount + 1; i++)
    {
        threads.emplace_back(worker , i);
        //std::cout << "Sent out worker:  " << i << std::endl;
    }

    for (auto& thread : threads) { // when a thread finishes
        thread.join();
    }
    DisplayThread.join();

    //stop time
    auto EndTime = std::chrono::high_resolution_clock::now();
    auto durationChrono = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime);

    for (uint16_t i = 0; i < Target-1;i++)//print out output
    {
        std::cout << i << ":"<< result[i] << std::endl;
    }
    std::cout << "Done in: " << durationChrono.count() << "   MicroSeconds using " << CPUthreadCount << "  Threads " << std::endl;
}