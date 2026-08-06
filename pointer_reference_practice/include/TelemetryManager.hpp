#pragma once

#include "Sensor.hpp"
#include <memory>

class TelemetryManager
{
public:
    explicit TelemetryManager(
        std::unique_ptr<Sensor> sensor
    );

    void readSensor() const;

private:
    std::unique_ptr<Sensor> sensor_;
};