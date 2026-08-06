#pragma once

#include "TelemetrySample.hpp"
#include <cstddef>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

class TelemetryLog
{
public:
    void addSample(float time, float speed, float rpm, float throttle, float brake);

    void printSamples() const;

    std::optional<float> averageSpeed() const;

    std::optional<float> maxRpm() const;

    std::optional<TelemetrySample> fastestSample() const;

    std::optional<TelemetrySample> firstSampleAboveRpmLimit(float rpmLimit) const;

    std::optional<TelemetrySample> firstHeavyBrakingEvent(float brakeLimit, float minimumSpeed) const;

    std::size_t countSamplesAboveRpm(float rpmLimit) const;

    void sortBySpeedDescending();

    bool saveToCsv(const std::string& filename) const;

    bool loadFromCsv(const std::string& filename);


    //HELPERS

    bool empty() const;

private:
    static bool isValidSample(const TelemetrySample& sample);

    static std::optional<TelemetrySample> parseCsvLine(const std::string& line);

    static void writeCsvLine(std::ofstream& file, const TelemetrySample& sample);

    static bool readAndValidateCsvHeader(std::ifstream& file);

    std::vector<TelemetrySample> samples;
};