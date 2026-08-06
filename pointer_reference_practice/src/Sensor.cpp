#include "Sensor.hpp"
#include <iostream>

Sensor::Sensor(int id)
    : id_(id)
{
    std::cout << "Sensor created\n";
}

Sensor::~Sensor()
{
    std::cout << "Sensor destroyed\n";
}

void Sensor::read() const
{
    std::cout << "Reading sensor " << id_ << '\n';
}