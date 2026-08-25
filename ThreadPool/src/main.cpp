#include <future>

#include "ThreadPool.h"

#include <iostream>
#include <future>
#include <memory>

int readRPM()
{
    std::cout << "Reading RPM\n";
    return 8500;
}

float readTemperature()
{
    std::cout << "Reading temperature\n";
    return 92.5f;
}

int main()
{
    ThreadPool pool(4);

    auto rpm = pool.submit(readRPM);

    auto temperature = pool.submit(readTemperature);



    std::cout << "RPM result: "
              << rpm.get()
              << '\n';

    std::cout << "Temperature result: "
              << temperature.get()
              << '\n';


    return 0;
}
