#include "ThreadPool.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int slowJob(int id)
{
    std::this_thread::sleep_for(
        std::chrono::seconds(2)
    );

    std::cout << "Job " << id << " finished\n";

    return id * 10;
}

int main()
{
    ThreadPool pool(4);

    std::vector<std::future<int>> results;

    auto start = std::chrono::steady_clock::now();

    for (int i = 1; i <= 8; ++i)
    {
        results.push_back(
            pool.submit([i]
            {
                return slowJob(i);
            })
        );
    }

    for (auto& result : results)
    {
        std::cout << "Result: "
                  << result.get()
                  << '\n';
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(
            end - start
        );

    std::cout << "Total time: "
              << elapsed.count()
              << " seconds\n";
}
