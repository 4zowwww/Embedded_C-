#include "TelemetryLog.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <cassert>

void TelemetryLog::addSample(float time, float speed, float rpm, float throttle, float brake)
{
    TelemetrySample sample{ time, speed, rpm, throttle, brake};

    if (!isValidSample(sample))
    {
        std::cout << "Invalid telemetry sample rejected\n";
        return;
    }

    samples.push_back(sample);
}

void TelemetryLog::printSamples() const
{
    if (empty())
    {
        std::cout << "No telemetry samples\n";
        return;
    }

    for (const TelemetrySample& sample : samples)
    {
        std::cout << "t=" << sample.time << " speed=" << sample.speed << " rpm=" << sample.rpm << " throttle=" << sample.throttle << " brake=" << sample.brake << "\n";
    }
}

std::optional<float> TelemetryLog::averageSpeed() const
{
    if (empty())
    {
        return std::nullopt;
    }

    float sum = 0.0f;

    for (const TelemetrySample& sample : samples)
    {
        sum += sample.speed;
    }

    return sum / samples.size();
}

std::optional<float>  TelemetryLog::maxRpm() const
{
    if(empty())
    {
        return std::nullopt;
    }

    auto result = std::max_element(
        samples.begin(),
        samples.end(),
        [](const TelemetrySample& first, const TelemetrySample& second)
         {
            return first.rpm < second.rpm;
         }
    );

    return (*result).rpm;
}

std::optional<TelemetrySample> TelemetryLog::fastestSample() const
{
    if(empty())
    {
        return std::nullopt;
    }

    auto result = std::max_element(
        samples.begin(),
        samples.end(),
        [](const TelemetrySample& first, const TelemetrySample& second)
         {
            return first.speed < second.speed;
         }
    );

    return *result;
}

std::optional<TelemetrySample> TelemetryLog::firstSampleAboveRpmLimit(float rpmLimit) const
{
    auto result = std::find_if(
        samples.begin(),
        samples.end(),
        [rpmLimit](const TelemetrySample& sample)
        {
            return sample.rpm > rpmLimit;
        }
    );

    if (result == samples.end())
    {
        return std::nullopt;
    }

    return *result;
}

std::optional<TelemetrySample> TelemetryLog::firstHeavyBrakingEvent(float brakeLimit, float minimumSpeed) const
{
    auto result = std::find_if(
        samples.begin(),
        samples.end(),
        [brakeLimit, minimumSpeed](const TelemetrySample& sample)
        {
            return sample.brake > brakeLimit && sample.speed > minimumSpeed;
        }
    );

    if (result == samples.end())
    {
        return std::nullopt;
    }

    return *result;
}

std::size_t TelemetryLog::countSamplesAboveRpm(float rpmLimit) const
{   
    int count = std::count_if(
        samples.begin(),
        samples.end(),
        [rpmLimit](const TelemetrySample& sample)
        {
            return sample.rpm > rpmLimit;
        }
    );

    return static_cast<std::size_t>(count);
}

void TelemetryLog::sortBySpeedDescending()
{
   std::sort(
         samples.begin(),
         samples.end(),
         [](const TelemetrySample& first, const TelemetrySample& second)
          {
             return first.speed > second.speed;
          }
    );
}

bool TelemetryLog::saveToCsv(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file)
    {
        return false;
    }

    file << "time,speed,rpm,throttle,brake\n";

    for (const TelemetrySample& sample : samples)
    {
        writeCsvLine(file, sample);
    }

    file.flush(); //.... buffer = data waiting to be written, flush  = push the waiting data out now

    return static_cast<bool>(file);
}

bool TelemetryLog::loadFromCsv(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file)
    {
        return false;
    }
   
    if (!readAndValidateCsvHeader(file))
    {
        return false;
    }

    std::string line;
    std::vector<TelemetrySample> loadedSamples;

    while(std::getline(file, line))
    {   
        if (line.empty())
        {
            continue;
        }

        std::optional<TelemetrySample> parsedSample = parseCsvLine(line);
        
        if (!parsedSample.has_value())
        {
            return false;
        }

        loadedSamples.push_back(parsedSample.value());
    }

    if (file.bad())
    {
        return false;
    }

    samples = std::move(loadedSamples);

    return true;
}



//HELPERS

bool TelemetryLog::empty() const 
{
    return samples.empty();
}

void TelemetryLog::writeCsvLine(std::ofstream& file, const TelemetrySample& sample)
{
    file << sample.time << ","
         << sample.speed << ","
         << sample.rpm << ","
         << sample.throttle << ","
         << sample.brake << "\n";
}

std::optional<TelemetrySample> TelemetryLog::parseCsvLine(const std::string& line)
{
    std::stringstream row(line);

    TelemetrySample sample{};

    char comma1;
    char comma2;
    char comma3;
    char comma4;

    if (!(row >> sample.time
              >> comma1
              >> sample.speed
              >> comma2
              >> sample.rpm
              >> comma3
              >> sample.throttle
              >> comma4
              >> sample.brake))
    {
        return std::nullopt;
    }

    if (comma1 != ',' ||
        comma2 != ',' ||
        comma3 != ',' ||
        comma4 != ',')
    {
        return std::nullopt;
    }

    if (!isValidSample(sample))
    {
        return std::nullopt;
    }

    return sample;
}

bool TelemetryLog::isValidSample(const TelemetrySample& sample)
{
    if (sample.time < 0.0f)
    {
        return false;
    }

    if (sample.speed < 0.0f)
    {
        return false;
    }

    if (sample.rpm < 0.0f)
    {
        return false;
    }

    if (sample.throttle < 0.0f ||
        sample.throttle > 100.0f)
    {
        return false;
    }

    if (sample.brake < 0.0f ||
        sample.brake > 100.0f)
    {
        return false;
    }

    return true;

}

bool TelemetryLog::readAndValidateCsvHeader(std::ifstream& file)
{
    std::string header;

    if (!std::getline(file, header))
    {
        return false;
    }

    return header == "time,speed,rpm,throttle,brake";
}


