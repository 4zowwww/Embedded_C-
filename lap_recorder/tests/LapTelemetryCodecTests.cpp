#include "LapTelemetryCodec.hpp"
#include "LapRecord.hpp"
#include <cassert>

void CodecTests()
{
        //stores no data, so no need for empty test


    {
        LapTelemetryCodec::Packet packet{};
        LapRecord input{4, 45.6f, 6.45f, 90.0f};
        LapRecord output{};
        
        const bool encoded = LapTelemetryCodec::encode(input, packet); //This converts the                                                             //record into eight raw bytes
        assert(encoded);
        assert((packet[7] & 0x01U) == 0x00U);

        const bool decoded = LapTelemetryCodec::decode(packet, output); //This reconstructs                                                         // a LapRecord
        assert(decoded);

        assert(output.lapNumber == input.lapNumber);
        assert(output.lapTime == input.lapTime);
        assert(output.fuelUsed == input.fuelUsed);
        assert(output.tireTemp == input.tireTemp);
    }


    {
        LapTelemetryCodec::Packet overheatPacket{};
        LapRecord input_2{4, 45.6f, 6.45f, 101.0f};

        const bool overheatEncoded = LapTelemetryCodec::encode(input_2, overheatPacket);
        assert(overheatEncoded);
        assert((overheatPacket[7] & 0x01U) == 0x01U);  
    }

    {
        LapRecord invalidLap{0, 45.6F, 6.45F, 90.0F};
        LapTelemetryCodec::Packet invalidPacket{};
        const bool invalidEncoded = LapTelemetryCodec::encode(invalidLap, invalidPacket);

        assert(!invalidEncoded);
    }

    {
        LapRecord invalidFuel{5, 45.6F, -1.0F, 90.0F};
        LapTelemetryCodec::Packet invalidFuelPacket{};
        const bool invalidFuelEncoded = LapTelemetryCodec::encode(invalidFuel, invalidFuelPacket);

        assert(!invalidFuelEncoded);
    }

    {
        LapRecord big_endian_input{300, 45.6F, 1.0F, 90.0F}; //300 = 0x012C
        LapTelemetryCodec::Packet BigEndianPacket{};
        const bool BigEndian = LapTelemetryCodec::encode(big_endian_input, BigEndianPacket);
        
        assert(BigEndian);
        assert(BigEndianPacket[0] ==  0x01U);
        assert(BigEndianPacket[1] ==  0x2CU);
    }


    
    {
        LapTelemetryCodec::Packet corruptedPacket{};
        const LapRecord validInput{5, 45.6F, 6.45F, 90.0F};
        LapRecord decodedOutput{};

        const bool encoded =
            LapTelemetryCodec::encode(validInput, corruptedPacket);

        assert(encoded);

        corruptedPacket[7] |= 0x02U;

        const bool decoded =
            LapTelemetryCodec::decode(corruptedPacket, decodedOutput);

        assert(!decoded);
    }
    
}