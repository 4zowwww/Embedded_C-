#include "StintLog.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <cassert>

bool StintLog::validate_laps(const LapRecord& sample)
{
    if (sample.lapNumber < 0)
    {
        return false;
    }

    if (sample.lapTime < 0.0f)
    {
        return false;
    }

    if (sample.fuelUsed < 0.0f || sample.fuelUsed > 100.0f)
    {
        return false;
    }

    if (sample.tireTemp < 0.0f)
    {
        return false;
    }

    return true;

}

bool StintLog::empty() const
{
    return samples.empty();
}

bool StintLog::add_laps(int lapNumber, float lapTime, float fuelUsed, float tireTemp)
{
    LapRecord sample{
        lapNumber,
        lapTime,
        fuelUsed,
        tireTemp
    };

    if (!validate_laps(sample))
    {
        return false;
    }

    samples.push_back(sample);

    return true;
}

std::optional<float> StintLog::average_lap_time() const
{
    if (samples.empty())
    {
        return std::nullopt;
    }

    float sum_laps = 0.0f;

    for (const LapRecord& sample : samples)
    {
        sum_laps += sample.lapTime;
    }

    return sum_laps /
           static_cast<float>(samples.size());
}

std::optional<LapRecord> StintLog::fastest_lap() const
{
    if(samples.empty())
    {
        return std::nullopt;
    }

    auto result = std::min_element(
        samples.begin(),
        samples.end(),
        [](const LapRecord& first, const LapRecord& second)
        {
            return first.lapTime < second.lapTime;
        }
    );

    return *result;
}

std::optional<LapRecord> StintLog::first_overheated_lap(float tireTemp_limit) const
{
    if(samples.empty())
    {
        return std::nullopt;
    }

    auto result = std::find_if(
        samples.begin(),
        samples.end(),
        [tireTemp_limit](const LapRecord& sample)
        {
            return sample.tireTemp > tireTemp_limit;
        }
    );

    if (result == samples.end())
    {
        return std::nullopt;
    }
    
    return *result;
}

void StintLog::sorted_laps_toSlowest() 
{
    std::sort(
        samples.begin(),
        samples.end(),
        [](const LapRecord& first, const LapRecord& second)
        {
            return first.lapTime < second.lapTime;
        }
    );
}

bool StintLog::save_laps(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file)
    {
        return false;
    }

    file << "lapNumber,lapTime,fuelUsed,tireTemp\n";

    for (const LapRecord& sample : samples)
    {
         file << sample.lapNumber << ","
         << sample.lapTime << ","
         << sample.fuelUsed << ","
         << sample.tireTemp << "\n";
    }

    file.flush();

    return static_cast<bool>(file);
}

bool StintLog::load_laps(const std::string& filename) 
{
    std::ifstream file(filename);

    if (!file)
    {
        return false;
    }

    std::string header;

    if (!std::getline(file, header))
    {
        return false;
    }

    if (header != "lapNumber,lapTime,fuelUsed,tireTemp")
    {
        return false;
    }

    std::string line;
    std::vector<LapRecord> loadedSamples;

    while(std::getline(file, line))
    {
        if(line.empty())
        {
            continue;
        }

        std::stringstream row(line);
        LapRecord sample{};

        char comma1;
        char comma2;
        char comma3;


        if (!(row >> sample.lapNumber
                >> comma1
                >> sample.lapTime
                >> comma2
                >> sample.fuelUsed
                >> comma3
                >> sample.tireTemp))
        {
            return false;
        }

        if (comma1 != ',' ||
            comma2 != ',' ||
            comma3 != ',')
        {
            return false;
        }

        if (!validate_laps(sample))
        {
            return false;
        }

        loadedSamples.push_back(sample);
    }
    
    if (file.bad())
    {
        return false;
    }

    samples = std::move(loadedSamples);

    return true;
}