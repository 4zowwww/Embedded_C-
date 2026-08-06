#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <array>
#include <optional>

enum class TemperatureStatus : std::uint8_t
{
    Normal,
    Warning,
    Critical,
    Invalid
};

TemperatureStatus evaluate_temperature(float temperature)
{
    if (temperature < -50.0f || temperature > 250.0f)
    {
        return TemperatureStatus::Invalid;
    }

    if (temperature < 90.0f)
    {
        return TemperatureStatus::Normal;
    }

    if (temperature <= 110.0f)
    {
        return TemperatureStatus::Warning;
    }

    return TemperatureStatus::Critical;
}

const char* status_to_string(TemperatureStatus status)
{
    switch (status)
    {
        case TemperatureStatus::Invalid: return "Invalid input.";
        case TemperatureStatus::Normal: return "Normal temp.";
        case TemperatureStatus::Warning: return "High temp.";
        case TemperatureStatus::Critical: return "Critical temp.";
    }

    return "Unknown";
}


std::array<std::uint8_t, 2>
to_little_endian(std::uint16_t value)
{
    std::array<std::uint8_t, 2> bytes{};

    bytes[0] = static_cast<std::uint8_t>(value & 0xFFU);
    bytes[1] = static_cast<std::uint8_t>((value >> 8) & 0xFFU);

    return bytes;
}

std::uint16_t from_little_endian(
    const std::array<std::uint8_t, 2>& bytes)
{
    std::uint16_t lower = static_cast<std::uint16_t>(bytes[0]);
    std::uint16_t upper = static_cast<std::uint16_t>(bytes[1]) << 8;

    return lower | upper;
}

std::array<std::uint8_t, 2>
to_big_endian(std::uint16_t value)
{
    std::array<std::uint8_t, 2> bytes{};

    bytes[0] = static_cast<std::uint8_t>((value >> 8) & 0xFFU);

    bytes[1] = static_cast<std::uint8_t>(value & 0xFFU);

    return bytes;
}

std::uint16_t from_big_endian(
    const std::array<std::uint8_t, 2>& bytes)
{
    std::uint16_t upper =
        static_cast<std::uint16_t>(bytes[0]) << 8;

    std::uint16_t lower =
        static_cast<std::uint16_t>(bytes[1]);

    return upper | lower;
}



bool write_u16_little_endian(
    std::array<std::uint8_t, 8>& buffer,
    std::size_t offset,
    std::uint16_t value)
{
    if (offset + 1 >= buffer.size())
    {
        return false;
    }

    buffer[offset] =
        static_cast<std::uint8_t>(value & 0xFFU);

    buffer[offset + 1] =
        static_cast<std::uint8_t>((value >> 8) & 0xFFU);

    return true;
}

std::optional<uint16_t> read_u16_little_endian(
    const std::array<std::uint8_t, 8>& buffer,
    std::size_t offset)
{
    if (offset + 1 >= buffer.size())
    {
        return std::nullopt;
    }

    std::uint16_t lower =
        static_cast<std::uint16_t>(buffer[offset]);

    std::uint16_t upper =
        static_cast<std::uint16_t>(buffer[offset + 1]) << 8;

    return lower | upper;
}


int main()
{
    const std::vector<float> temperatures{
        25.0f,
        95.0f,
        125.0f,
        300.0f
    };  

    for (float temperature : temperatures)
    {
        TemperatureStatus temp_status = evaluate_temperature(temperature);
        const char* status2string = status_to_string(temp_status);

        std::cout << "TEMPERATURE: " << temperature << " | " <<  "STATUS: " <<  status2string << "\n";
    }

    std::uint16_t engine_rpm = 12450;
    std::uint8_t throttle_percent = 87;
    std::int16_t tire_temperature_tenths = 953;
    std::uint8_t decimal_value = 165;
    std::uint8_t binary_value = 0b10100101;
    std::uint8_t hexadecimal_value = 0xA5;

    std::cout << "Engine RPM: " << engine_rpm << '\n';

    std::cout << "Throttle: " << static_cast<int>(throttle_percent) << "%\n";

    std::cout << "Tire temperature: " << static_cast<float>(tire_temperature_tenths) / 10.0f << " C\n";

    std::cout << "Decimal literal: "
          << static_cast<int>(decimal_value) << '\n';

    std::cout << "Binary literal: "
            << static_cast<int>(binary_value) << '\n';

    std::cout << "Hex literal: "
            << static_cast<int>(hexadecimal_value) << '\n';

 

    std::uint8_t gear = 5;
    std::uint8_t driving_mode = 2;
    bool overheating = true;
    bool sensor_fault = false;
    std::uint16_t rpm = 12400;


    std::uint16_t telemetry = 0;


    telemetry |= static_cast<std::uint16_t>(gear);
    telemetry |= static_cast<std::uint16_t>(driving_mode) << 3;
    telemetry |= static_cast<std::uint16_t>(overheating) << 5;
    telemetry |= static_cast<std::uint16_t>(sensor_fault) << 6;
    telemetry |= static_cast<std::uint16_t>(rpm / 100) << 8;


    constexpr std::uint16_t gear_mask = 0b111U;
    constexpr unsigned gear_shift = 0;

    // Clear the old gear bits
    telemetry &= static_cast<std::uint16_t>(~gear_mask);

    // Insert the new gear
    telemetry |= static_cast<std::uint16_t>
    ((static_cast<std::uint16_t>(gear) & gear_mask) << gear_shift);

    constexpr std::uint16_t mode_value_mask = 0b11U;
    constexpr unsigned mode_shift = 3;

    constexpr std::uint16_t mode_position_mask =
        static_cast<std::uint16_t>(mode_value_mask << mode_shift);

    telemetry &= static_cast<std::uint16_t>(~mode_position_mask);

    telemetry |= static_cast<std::uint16_t>
    ((static_cast<std::uint16_t>(driving_mode) & mode_value_mask) << mode_shift);

    std::cout << "Packed telemetry: 0x"
          << std::hex
          << std::uppercase
          << telemetry
          << '\n';

    std::array<std::uint8_t, 8> data{};

    data[0] = static_cast<std::uint8_t>(telemetry & 0xFFU);
    data[1] = static_cast<std::uint8_t>((telemetry >> 8) & 0xFFU);

    std::uint16_t received_telemetry =
    static_cast<std::uint16_t>(data[0]) |
    static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[1]) << 8);

    std::cout << "Reconstructed: 0x" << received_telemetry << '\n';

    std::uint16_t received =
    static_cast<std::uint16_t>(data[0]) |
    static_cast<std::uint16_t>(data[1]) << 8;

    std::cout << "Little-endian buffer: 0x" << received << '\n';

    received =
    static_cast<std::uint16_t>(data[0]) << 8 |
    static_cast<std::uint16_t>(data[1]);

    std::cout << "Big-endian buffer: 0x" << received << '\n';


    std::uint16_t original = 0x327A;

    auto bytes = to_little_endian(original);
    std::uint16_t reconstructed = from_little_endian(bytes);

    std::cout << "LITTLE ENDIAN: " << reconstructed << "\n";


    
    bool round_trip_ok = (original == reconstructed);

    std::cout << std::boolalpha;
    std::cout << "Round trip successful: "
            << round_trip_ok << '\n';

    std::cout << std::noboolalpha;



    std::array<std::uint8_t, 2> big_bytes =
    to_big_endian(original);

    std::uint16_t big_reconstructed =
        from_big_endian(big_bytes);

    std::cout << std::hex << std::uppercase;

    std::cout << "Big-endian bytes: 0x"
            << static_cast<int>(big_bytes[0])
            << " 0x"
            << static_cast<int>(big_bytes[1])
            << '\n';

    std::cout << "Big-endian reconstructed: 0x"
            << big_reconstructed << '\n';

    std::cout << std::dec;

    bool big_round_trip_ok =
        (original == big_reconstructed);

    std::cout << std::boolalpha
            << "Big-endian round trip successful: "
            << big_round_trip_ok << '\n'
            << std::noboolalpha;

    

    std::array<std::uint8_t, 8> can_data{};

    rpm = 12450;

    bool write_success = write_u16_little_endian(can_data, 2, rpm);

    if (!write_success)
    {
        std::cout << "Buffer write failed\n";
    }

    std::optional<std::uint16_t> received_rpm =
    read_u16_little_endian(can_data, 2);


    if (received_rpm.has_value())
    {
        std::cout << "Received RPM: "
                << received_rpm.value() << '\n';
    }
    else
    {
        std::cout << "Buffer read failed\n";
    }

    return 0;
}