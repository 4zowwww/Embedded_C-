#include <iostream>
#include <memory>
#include "Sensor.hpp"
#include "TelemetryManager.hpp"
#include <utility> 

void changeCopy(int number)
{
    number = 100;
}

void changeOriginal(int& number)
{
    number = 100;
}



int main()
{

    {
        int first = 10;
        int second = 10;

        changeCopy(first);
        changeOriginal(second);

        std::cout << "First: " << first << '\n';
        std::cout << "Second: " << second << '\n';
    }




    {
        int value = 10;
        int* pointer = &value;

        std::cout << "Value: " << value << '\n';
        std::cout << "Dereferenced: " << *pointer << '\n';
    }




    {
        int first = 10;
        int second = 20;

        int* selected = nullptr;

        selected = &first;
        std::cout << *selected << '\n';

        selected = &second;
        std::cout << *selected << '\n';
    }




    {
        int* pointer = new int(10);

        std::cout << *pointer << '\n';

        delete pointer;
        pointer = nullptr;
    }



    {
        int stackValue = 10;

        int* rawPointer = new int(20);

        std::unique_ptr<int> smartPointer =
            std::make_unique<int>(30);

        std::cout << "Stack: " << stackValue << '\n';
        std::cout << "Raw heap: " << *rawPointer << '\n';
        std::cout << "Smart heap: " << *smartPointer << '\n';

        delete rawPointer;
        rawPointer = nullptr;
    }



    {
        std::unique_ptr<int> first =
            std::make_unique<int>(10);

        std::cout << "Before move: "
                << *first << '\n';

        std::unique_ptr<int> second =
            std::move(first);

        if (first == nullptr)
        {
            std::cout << "First no longer owns the value\n";
        }

        std::cout << "Second owns: "
                << *second << '\n';
    }


    {
        std::unique_ptr<Sensor> sensor =
            std::make_unique<Sensor>(1);

        sensor->read();  // ((*sensor).read())

        std::unique_ptr<Sensor> movedSensor =
            std::move(sensor);

        if (sensor == nullptr)
        {
            std::cout << "Original pointer is empty\n";
        }

        movedSensor->read();
    }




    {
        std::shared_ptr<int> first =
            std::make_shared<int>(10);

        std::shared_ptr<int> second = first;

        std::cout << *first << '\n';
        std::cout << *second << '\n';
    }


    {
        std::shared_ptr<Sensor> sensorA =
            std::make_shared<Sensor>(1);

        std::shared_ptr<Sensor> sensorB = sensorA;

        sensorA->read();
        sensorB->read();

        std::cout << "Owners: "
                << sensorA.use_count()
                << '\n';
    }

    {
        std::shared_ptr<Sensor> sensorA =
            std::make_shared<Sensor>(1);

        std::shared_ptr<Sensor> sensorB = sensorA;

        sensorA->read();
        sensorA.use_count();
        sensorB->read();
        sensorA.use_count();
        sensorB.reset();
        std::cout << "Owners: "
                << sensorA.use_count()
                << '\n';
    }




    {
        std::shared_ptr<Sensor> owner =
            std::make_shared<Sensor>(1);

        std::weak_ptr<Sensor> observer = owner;

        std::cout << "Owners: "
                << owner.use_count()
                << '\n';

        if (std::shared_ptr<Sensor> locked = observer.lock())
        {
            locked->read();
        }
    }


    return 0;
}