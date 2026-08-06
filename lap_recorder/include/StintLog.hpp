#pragma once

#include "LapRecord.hpp"
#include <cstddef>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

class StintLog 
{
    private:
        std::vector<LapRecord> samples;

        static bool validate_laps(const LapRecord& sample); //helper

    public:
        bool add_laps(int lapNumber, float lapTime, float fuelUsed, float tireTemp);

        std::optional<float> average_lap_time() const;

        std::optional<LapRecord> fastest_lap() const;

        std::optional<LapRecord> first_overheated_lap(float tireTemp_limit) const;

        void sorted_laps_toSlowest();

        bool save_laps(const std::string& filename) const;

        bool load_laps(const std::string& filename);

        bool empty() const; //helper


};
