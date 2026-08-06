#include "StintLog.hpp"
#include <iostream>
#include <optional>
#include <cassert>
#include <cmath>

void LogTests()
{   
    {
        StintLog emptyLog;

        assert(emptyLog.empty());
        assert(!emptyLog.average_lap_time().has_value());
    }




    {
    StintLog Log;

    Log.add_laps(1, 1.23, 3.45, 127.65);
    Log.add_laps(2, 2.41, 7.15, 140.70);
    Log.add_laps(3, 1.24, 13.16, 156.10);

    assert(!Log.empty());

    std::optional<float> average_lap_time_logs = Log.average_lap_time();

    assert(average_lap_time_logs.has_value());
    assert(std::fabs(average_lap_time_logs.value() - 1.6266667f) < 0.0001f); //assert(average_lap_time_logs.value() == 1.626);

    std::optional<LapRecord> fastest = Log.fastest_lap();

    assert(fastest.has_value());
    assert(fastest->lapNumber == 1);
    

    std::optional<LapRecord> firstOverheatedEvent = Log.first_overheated_lap(155.0);

    assert(firstOverheatedEvent.has_value());
    assert(firstOverheatedEvent.value().lapNumber == 3);
    assert(firstOverheatedEvent.value().lapTime == 1.24f);
    assert(firstOverheatedEvent.value().fuelUsed == 13.16f);
    assert(firstOverheatedEvent.value().tireTemp == 156.10f);


    assert(Log.save_laps("Laps_logs.csv"));

    bool read = Log.load_laps("Laps_logs.csv");

    assert(read);
    assert(!Log.empty());

    LapRecord loaded_logs;



    }

}