#include "LapTelemetryCodec.hpp"
#include <cmath>
#include <cstdint>

bool LapTelemetryCodec::encode(const LapRecord& record, 
    Packet& outputPacket)
{
    if( record.lapNumber < 1 || record.lapNumber > 65535)
    {
        return false;                                                        // 1. Validate
    }
    else if (record.lapTime <= 0.0f)
    {
        return false;
    }
    else if (record.fuelUsed < 0.0f)
    {
        return false;
    }
    else if (record.tireTemp < 0.0f || record.tireTemp > 255.0f)
    {
        return false;
    }
    
    

    const long newTime =
        std::lround(record.lapTime * 100.0F);

    if (newTime <= 0 || newTime > 65535)
    {
        return false;
    }
                                                                //2. Convert to
    const long newFuel =                                       // scalable intgrs
        std::lround(record.fuelUsed * 100.0F); //std::lround 
                                               //example .5 -> 1
    if (newFuel < 0 || newFuel > 65535)
    {
        return false;
    }


    const std::uint16_t lapNumber =
        static_cast<std::uint16_t>(record.lapNumber);

    outputPacket[0] =
        static_cast<std::uint8_t>((lapNumber >> 8) & 0xFFU);

    outputPacket[1] =
        static_cast<std::uint8_t>(lapNumber & 0xFFU);


    const std::uint16_t scaledTime =
    static_cast<std::uint16_t>(newTime);

    outputPacket[2] = 
        static_cast<std::uint8_t>((newTime >> 8) & 0xFFU);

    outputPacket[3] = 
        static_cast<std::uint8_t>(newTime & 0xFFU);             //3. store
 
        
    const std::uint16_t scaledFuel =
        static_cast<std::uint16_t>(newFuel);

    outputPacket[4] = 
        static_cast<std::uint8_t>((newFuel >> 8) & 0xFFU);

    outputPacket[5] = 
        static_cast<std::uint8_t>(newFuel & 0xFFU);
    

    const std::uint8_t tireTemp = 
        static_cast<std::uint8_t>(record.tireTemp);

    outputPacket[6] = 
        static_cast<std::uint16_t>(tireTemp & 0xFFU);


    std::uint16_t status_byte = 0U;

    if (record.tireTemp >= 100.0F)
    {
        status_byte |= 0x01U;
    }

    outputPacket[7] = status_byte;
    


    return true;
}

bool LapTelemetryCodec::decode(const Packet& packet,
    LapRecord& outputRecord)
{
    const std::uint16_t lapNumber =
        (static_cast<std::uint16_t>(packet[0]) << 8) |
        static_cast<std::uint16_t>(packet[1]);

    const std::uint16_t scaledTime =
        (static_cast<std::uint16_t>(packet[2]) << 8) |
        static_cast<std::uint16_t>(packet[3]);

    const std::uint16_t scaledFuel =
        (static_cast<std::uint16_t>(packet[4]) << 8) |
        static_cast<std::uint16_t>(packet[5]);


    if (lapNumber == 0 || scaledTime == 0)
    {
        return false;
    }

    if ((packet[7] & 0xFEU) != 0U) // XXXX XXXX & 1111 1110 or 0x00 & 0xFE
    {                              // for overheating
        return false;
    }
    
    outputRecord.lapNumber = static_cast<int>(lapNumber);
    outputRecord.lapTime = static_cast<float>(scaledTime) / 100.0F;
    outputRecord.fuelUsed = static_cast<float>(scaledFuel) / 100.0F;
    outputRecord.tireTemp = static_cast<float>(packet[6]);

    return true;
}