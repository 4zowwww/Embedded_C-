#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <array>
#include <optional>
#include "LapRecord.hpp"
#pragma once

class LapTelemetryCodec
{   
    public:
        using Packet = std::array<std::uint8_t, 8>;

        static bool encode(const LapRecord& record, Packet& OUTpacket);

        static bool decode(const Packet& packet, LapRecord& OUTrecord);
};