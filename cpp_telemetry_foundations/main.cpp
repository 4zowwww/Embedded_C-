#include "TelemetryLog.hpp"

#include <iostream>
#include <optional>
#include <cassert>

void runTelemetryTests()
{   
    {
        TelemetryLog emptyLog;

        assert(emptyLog.empty());
        assert(!emptyLog.averageSpeed().has_value());
    }

    {
        TelemetryLog log;

        log.addSample(0.0f, 80.0f, 5000.0f, 30.0f, 0.0f);
        log.addSample(0.1f, 90.0f, 5500.0f, 45.0f, 0.0f);
        log.addSample(0.2f, 100.0f, 6200.0f, 60.0f, 10.0f);

        assert(!log.empty());
        
        std::optional<float> average_speed = log.averageSpeed();

        assert(average_speed.has_value());
        assert(average_speed.value() == 90.0f);

        std::optional<float> max_rpm_sample = log.maxRpm();

        assert(max_rpm_sample.has_value());
        assert(max_rpm_sample.value() == 6200.0f);

        std::optional<TelemetrySample> fastest_sample = log.fastestSample();

        assert(fastest_sample.has_value());
        assert(fastest_sample.value().speed == 100.0f);

        std::optional<TelemetrySample> first_rpmLimit_sample = log.firstSampleAboveRpmLimit(6000.0f);

        assert(first_rpmLimit_sample.has_value());
        assert(first_rpmLimit_sample.value().time == 0.2f);
        assert(first_rpmLimit_sample.value().speed == 100.0f);
        assert(first_rpmLimit_sample.value().rpm == 6200.0f);
        assert(first_rpmLimit_sample.value().throttle == 60.0f);
        assert(first_rpmLimit_sample.value().brake == 10.0f);

        std::optional<TelemetrySample> first_braking_event = log.firstHeavyBrakingEvent(9.0f, 95.5f);

        assert(first_braking_event.has_value());
        assert(first_braking_event.value().speed == 100.0f);
        assert(first_braking_event.value().brake == 10.0f); 

        std::size_t samples_above_rpm_limit = log.countSamplesAboveRpm(6000.0f);

        assert(samples_above_rpm_limit == 1);

        bool write_logs = log.saveToCsv("telemetry.csv");

        assert(write_logs);
        
        TelemetryLog loadedLog;

        bool read_log = loadedLog.loadFromCsv("telemetry.csv");

        assert(read_log);           //CHECKS: MISSING FILE, FIRST LINE EXISTS, VALID VALUES, READING FAILURE (loading process succeeded).......
        assert(!loadedLog.empty()); //CHECKS: at least one sample..... 


        //CSV functionality
        auto loadedAverage = loadedLog.averageSpeed();
        assert(loadedAverage.has_value());
        assert(loadedAverage.value() == 90.0f);

        auto loadedMaximumRpm = loadedLog.maxRpm();
        assert(loadedMaximumRpm.has_value());
        assert(loadedMaximumRpm.value() == 6200.0f);

        auto loadedFastest = loadedLog.fastestSample();
        assert(loadedFastest.has_value());
        assert(loadedFastest.value().speed == 100.0f);
    }   
}

int main()
{
    // Development checks
    runTelemetryTests();

    std::cout << "All telemetry tests passed.\n\n";

    // 1. Create a telemetry log
    TelemetryLog log;

    // 2. Add telemetry samples
    log.addSample(0.0f, 80.0f, 5000.0f, 30.0f, 0.0f);
    log.addSample(0.1f, 90.0f, 5500.0f, 45.0f, 0.0f);
    log.addSample(0.2f, 100.0f, 6200.0f, 60.0f, 10.0f);

    // 3. Display the original samples
    std::cout << "Original telemetry:\n";
    log.printSamples();

    // 4. Calculate and display average speed
    std::optional<float> averageSpeed = log.averageSpeed();

    if (averageSpeed.has_value())
    {
        std::cout << "\nAverage speed: "
                  << averageSpeed.value()
                  << "\n";
    }

    // 5. Save the log
    bool saved = log.saveToCsv("telemetry.csv");

    if (!saved)
    {
        std::cerr << "Failed to save telemetry.csv\n";
        return 1;
    }

    std::cout << "Telemetry saved successfully.\n";

    // 6. Create a separate empty log
    TelemetryLog loadedLog;

    // 7. Load the CSV into the new log
    bool loaded = loadedLog.loadFromCsv("telemetry.csv");

    if (!loaded)
    {
        std::cerr << "Failed to load telemetry.csv\n";
        return 1;
    }

    // 8. Display the samples reconstructed from the CSV
    std::cout << "\nTelemetry loaded from CSV:\n";
    loadedLog.printSamples();

    return 0;
}