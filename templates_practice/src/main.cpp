#include <iostream>
#include "SensorValue.hpp"
#include "SensorBuffer.hpp"
#include <queue>

template <typename T>
T maximum(T first, T second)
{
    return first > second ? first : second;
}

template <typename T>
T minimum(T first, T second)
{
    return first < second ? first : second;
}

template <typename T>
T clampValue(T value, T minimumValue, T maximumValue)
{
    if (value < minimumValue)
    {
        return minimumValue;
    }

    if (value > maximumValue)
    {
        return maximumValue;
    }

    return value;
}

int main()
{
    int largestLap = maximum(4, 7);

    float largestTemperature =
        maximum(85.5F, 92.3F);

    std::cout << "Largest lap: "
              << largestLap << '\n';

    std::cout << "Largest temperature: "
              << largestTemperature << '\n';

    
    int smallestLap = minimum(4, 7);

    float lowestTemperature =
        minimum(85.5F, 92.3F);

    std::cout << "Smallest lap: "
            << smallestLap << '\n';

    std::cout << "Lowest temperature: "
            << lowestTemperature << '\n';



    int throttleCommand =
        clampValue(120, 0, 100);

    float tireTemperature =
        clampValue(115.5F, 20.0F, 110.0F);

    std::cout << "Throttle command: "
            << throttleCommand << '\n';

    std::cout << "Clamped tire temperature: "
            << tireTemperature << '\n';



    {
        SensorValue<float> tireTemperature{0.0F, 150.0F};

        SensorValue<int> engineRpm{0, 15000};

        bool temperatureAccepted =
            tireTemperature.set(92.5F);

        bool rpmAccepted =
            engineRpm.set(8500);

        std::cout << std::boolalpha;

        std::cout << "Temperature accepted: "
                << temperatureAccepted << '\n';

        std::cout << "RPM accepted: "
                << rpmAccepted << '\n';

        if (const auto& temperature = tireTemperature.get())
        {
            std::cout << "Tire temperature: "
                    << *temperature << '\n';
        }

        if (const auto& rpm = engineRpm.get())
        {
            std::cout << "Engine RPM: "
                    << *rpm << '\n';
        }
    }



    {
        SensorValue<float> tireTemperature{0.0F, 150.0F};

        std::cout << std::boolalpha;

        std::cout << "Has value initially: "
                << tireTemperature.hasValue() << '\n';

        tireTemperature.set(92.5F);

        std::cout << "Has value after set: "
                << tireTemperature.hasValue() << '\n';

        if (const auto& temperature = tireTemperature.get())
        {
            std::cout << "Temperature: "
                    << *temperature << " C\n";
        }

        tireTemperature.clear();

        std::cout << "Has value after clear: "
                << tireTemperature.hasValue() << '\n';
    }




    {
        SensorBuffer<float, 3> temperatures;

        std::cout << std::boolalpha;

        std::cout << "Add 90: "
                << temperatures.add(90.0F) << '\n';

        std::cout << "Add 91: "
                << temperatures.add(91.0F) << '\n';

        std::cout << "Add 92: "
                << temperatures.add(92.0F) << '\n';

        std::cout << "Add 93: "
                << temperatures.add(93.0F) << '\n';

        std::cout << "Size: "
                << temperatures.size() << '\n';

        std::cout << "Full: "
                << temperatures.full() << '\n';

        if (auto reading = temperatures.get(1))
        {
            std::cout << "Reading at index 1: "
                    << *reading << '\n';
        }

        if (auto reading = temperatures.get(5))
        {
            std::cout << *reading << '\n';
        }
        else
        {
            std::cout << "No reading at index 5\n";
        }

        if (auto value = temperatures.latest())
        {
            std::cout << "Latest temperature: "
                    << *value << '\n';
        }

        if (auto result = temperatures.average())
        {
            std::cout << "Average temperature: "
                    << *result << '\n';
        }

        temperatures.clear();

        std::cout << "Size after clear: "
                << temperatures.size() << '\n';

        if (!temperatures.get(0))
        {
            std::cout << "No reading after clear\n";
        }


        temperatures.add(90.0F);
        temperatures.add(91.0F);
        temperatures.add(92.0F);
        temperatures.add(93.0F);

        for (std::size_t index = 0;
            index < temperatures.size();
            ++index)
        {
            if (auto reading = temperatures.get(index))
            {
                std::cout << "Reading " << index
                        << ": " << *reading << '\n';
            }
        }

        if (auto latest = temperatures.latest())
        {
            std::cout << "Latest: "
                    << *latest << '\n';
        }

        if (auto average = temperatures.average())
        {
            std::cout << "Average: "
                    << *average << '\n';
        }

        if (auto value = temperatures.oldest())
        {
            std::cout << "Oldest: "
                    << *value << '\n';
        }

        std::cout << "Capacity: "
          << temperatures.capacity() << '\n';

        if (auto value = temperatures.minimum())
        {
            std::cout << "Minimum: "
                    << *value << '\n';
        }

        if (auto value = temperatures.maximum())
        {
            std::cout << "Maximum: "
                    << *value << '\n';
        }

        std::cout << std::boolalpha;

        std::cout << "Contains 92: "
                << temperatures.contains(92.0F) << '\n';

        std::cout << "Contains 50: "
                << temperatures.contains(50.0F) << '\n';

        std::cout << "Readings above 91: "
          << temperatures.countAbove(91.0F) << '\n';
    }


    {
        std::queue<float> pendingTemperatures;

        pendingTemperatures.push(90.0F);
        pendingTemperatures.push(91.0F);
        pendingTemperatures.push(92.0F);

        std::cout << "Queue size: "
                << pendingTemperatures.size() << '\n';

        std::cout << "Oldest pending reading: "
                << pendingTemperatures.front() << '\n';

        std::cout << "Newest pending reading: "
                << pendingTemperatures.back() << '\n';

        while (!pendingTemperatures.empty())
        {
            const float reading =
                pendingTemperatures.front();

            std::cout << "Processing: "
                    << reading << '\n';

            pendingTemperatures.pop();
        }

        std::cout << "Queue empty: "
                << std::boolalpha
                << pendingTemperatures.empty()
                << '\n';
    }

    


    return 0;
}
