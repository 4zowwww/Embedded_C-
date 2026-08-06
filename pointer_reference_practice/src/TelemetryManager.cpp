#include "TelemetryManager.hpp"
#include <utility>

TelemetryManager::TelemetryManager(
    std::unique_ptr<Sensor> sensor
)
    : sensor_(std::move(sensor))
{
    
}

void TelemetryManager::readSensor() const
{
    sensor_->read();
}