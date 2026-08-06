#include "StintLog.hpp" 
#include "LapTelemetryCodec.hpp"
#include <iostream>
#include <optional>
#include <string>
#include "SensorBuffer.hpp"
#include <queue>


void LogTests(); 

 void CodecTests();

int main()
{      
        LogTests(); 
        CodecTests();


        StintLog Log;
        SensorBuffer<float, 5> recentLapTimes;
        std::queue<LapRecord> pendingLaps;  

        bool recording = true;

        while(recording)
        {   
            int lapNumber_m;
            float lapTime_m;
            float fuelUsed_m;
            float tireTemp_m;


            std::cout << 
                "Enter Lap data (lap number time fuel-used tire-temperature): ";

            if (!(std::cin >> lapNumber_m >> lapTime_m >> fuelUsed_m >> tireTemp_m))
            {
                std::cerr << "Invalid keyboard input.\n";
                return 1;
            }


            LapRecord input{lapNumber_m, lapTime_m, fuelUsed_m, tireTemp_m};
            LapTelemetryCodec::Packet packet{};
            LapRecord output{};

            if(!LapTelemetryCodec::encode(input, packet))
            {
                std::cerr << "Encoding failed. \n";
                continue;
            }

            if(!LapTelemetryCodec::decode(packet, output))
            {
                std::cerr << "Decoding failed. \n";
                continue;
            }
            else
            {
                pendingLaps.push(output);

                std::cout << "Lap placed in processing queue.\n";
            }



            char answer{};  //safer then jsut answer because answer{} = null

            std::cout << "Add another lap? (y/n): ";
            std::cin >> answer;

            if (answer != 'y' && answer != 'Y')
            {
                recording = false;
            }
        }

    while (!pendingLaps.empty())
        {
            LapRecord reading =
                pendingLaps.front();

            pendingLaps.pop();

            const bool added = Log.add_laps(
                reading.lapNumber,
                reading.lapTime,
                reading.fuelUsed,
                reading.tireTemp);

            if (!added)
            {
                std::cerr << "Could not store lap \n";
                continue;
            }
            
            recentLapTimes.add(reading.lapTime);

            std::cout << "Lap "
                << reading.lapNumber
                << " processed.\n";
                            
        }        
        
    for (std::size_t index=0; index < recentLapTimes.size(); index++)
    {   
        std::optional<float> index_LAP_TIME = recentLapTimes.get(index);

        if(!(index_LAP_TIME.has_value()))
        {
            std::cerr << "Lap: " << index << " empty. \n";
            continue;
        }

        std::cout<< "Lap-time history " << index << ": " 
                << *index_LAP_TIME << "\n";     
    }
    

        

    std::optional<float> avarageONlapTIME = Log.average_lap_time();
    std::optional<LapRecord> fastest = Log.fastest_lap();

    if( avarageONlapTIME.has_value() && fastest.has_value() )
    {
        std::cout << "avarage lap time: " <<  *avarageONlapTIME << " | fastest lap: " << fastest -> lapNumber << "\n";
    }
    else
    {
        std::cerr << "no data on average lap time. \n";
    }

    
    
    std::string filename = "data/saved_laps.csv";

    bool saved_lap = Log.save_laps(filename);

    if( saved_lap )
    {
        std::cout << "saved seccesfully. \n";
    }
    else
    {
        std::cerr << "unable to save. \n";
    }

    StintLog loadedLog;

    bool loaded_laps = loadedLog.load_laps(filename);

    if (!loaded_laps)
    {
        std::cerr << "Unable to load laps.\n";
        return 1;
    }

    std::cout << "Laps loaded successfully.\n";

    
    return 0;
}