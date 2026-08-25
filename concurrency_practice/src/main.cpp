#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include "ThreadSafeQueue.hpp"
#include <atomic>
#include <future>



std::mutex mutexA;
std::mutex mutexB;
std::atomic<int> counter = 0;
std::atomic<bool> running = true;
std::atomic<int> value{10};
std::atomic<int> motorPower{90};
std::atomic<int> counter2{0};
std::atomic<bool> ready{false};
int sensorData = 0;

int calculatePower(int voltage, int current)
{
    return voltage * current;
}

float calculateAverageRPM(int rpm1, int rpm2) {

    float  average = (rpm1 + rpm2) / 2.0f;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    return average;
}

void calculateMotorTemperature(std::promise<float> temperaturePromise)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));

    float temperature = 87.5f;

    temperaturePromise.set_value(temperature);
}

struct Telemetry {
    int rpm;
    float temperature;
};

Telemetry telemetry;
std::atomic<bool> telemetryReady{false};

void producer3() {
    telemetry.rpm = 8500;
    telemetry.temperature = 92.5f;

    telemetryReady.store(true, std::memory_order_release);
}
void consumer3() {
    while (!telemetryReady.load(std::memory_order_acquire)) {}

    std::cout << "rpm" << telemetry.rpm << '\n';
    std::cout << "temperature" << telemetry.temperature << '\n';

}

void producer2() {
     sensorData = 42;

    ready.store(
        true,
        std::memory_order_release
    );
}

void consumer2() {
    while (!ready.load(std::memory_order_acquire)) {
    }

    std::cout << "Sensor data: "
              << sensorData
              << '\n';
}

void incrementCounter() {
    for (int i = 0; i < 100000; ++i) {
        counter.fetch_add(1,std::memory_order_relaxed);
    }
}


void increasePower(int amount, int id) {

    int current = motorPower.load();

    while (true) {

        int desired = current + amount;
        int failures = 0;

        if (desired > 100) {
            desired = 100;
        }

        if (motorPower.compare_exchange_weak(
                current,
                desired))
        {
            std::cout << "Thread " << id
                      << " succeeded after "
                      << failures << " failures\n";

            break;
        }

        failures++;

        std::cout << "Thread " << id
                  << " CAS failed, new current = "
                  << current << '\n';
    }
}

void incrementCounter2()
{
    for (int i = 0; i < 100000; ++i)
    {
        counter.fetch_add(1);
    }
}

void backgroundWorker()
{
    while (running.load())
    {
        std::cout << "Working...\n";

        std::this_thread::sleep_for(
            std::chrono::milliseconds(200)
        );
    }

    std::cout << "Worker stopped\n";
}


void worker1()
{
    std::scoped_lock lock(mutexA, mutexB);

    std::cout << "Worker 1 finished\n";
}

void worker2()
{
    std::scoped_lock lock(mutexA, mutexB);

    std::cout << "Worker 2 finished\n";
}

int main()
{
    ThreadSafeQueue queue;

    std::thread producerThread1(&ThreadSafeQueue::producer, &queue, 1);
    std::thread producerThread2(&ThreadSafeQueue::producer, &queue, 2);

    std::thread consumerThread1(&ThreadSafeQueue::consumer, &queue, 1);
    std::thread consumerThread2(&ThreadSafeQueue::consumer, &queue, 2);

    producerThread1.join();
    producerThread2.join();

    queue.close();

    consumerThread1.join();
    consumerThread2.join();

    std::thread t1(worker1);
    std::thread t2(worker2);

    t1.join();
    t2.join();


    std::thread c1(incrementCounter2);
    std::thread c2(incrementCounter2);

    c1.join();
    c2.join();

    std::cout << "Counter: " << counter << '\n';

    std::thread b3(backgroundWorker);

    std::this_thread::sleep_for(
    std::chrono::seconds(1)
);

    running.store(false);

    b3.join();




    int expected = 10;

    bool success = value.compare_exchange_strong(expected, 20);

    std::cout << "Success: " << success << '\n';
    std::cout << "Value: " << value.load() << '\n';




    increasePower(20, 0);

    std::cout << motorPower.load() << '\n';




    std::thread p1(increasePower, 20, 1);
    std::thread p2(increasePower, 25, 2);
    std::thread p3(increasePower, 15, 3);

    p1.join();
    p2.join();
    p3.join();

    std::cout << motorPower.load() << '\n';




    std::thread i1(incrementCounter);
    std::thread i2(incrementCounter);
    std::thread i3(incrementCounter);

    i1.join();
    i2.join();
    i3.join();

    std::cout << "Counter: "
              << counter.load(std::memory_order_relaxed)
              << '\n';



    std::thread f1(producer2);
    std::thread f2(consumer2);

    f1.join();
    f2.join();



    std::thread g1(producer3);
    std::thread g2(consumer3);

    g1.join();
    g2.join();


    std::promise<float> temperaturePromise;

    std::future<float> temperatureFuture = temperaturePromise.get_future();

    std::thread worker11(calculateMotorTemperature, std::move(temperaturePromise));

    std::cout << "Calculating...\n";

    float temperature = temperatureFuture.get();

    std::cout << "Motor temperature: "
              << temperature
              << " C\n";

    worker11.join();






    std::future<float> averageRPM = std::async(std::launch::async, calculateAverageRPM,  5000, 6000);

    std::cout << "Calculating...\n";

    float rpm = averageRPM.get();

    std::cout << "average RPM: " << rpm << '\n';




    std::packaged_task<int(int, int)> task(calculatePower);

    std::future<int> result = task.get_future();

    task(12, 5);

    std::cout << result.get() << '\n';




    std::packaged_task<int(int, int)> rpmTask(calculateAverageRPM);

    std::future<int> rpmFuture = rpmTask.get_future();

    std::thread worker1111(std::move(rpmTask), 5000, 6000);

    std::cout << "Average RPM: " << rpmFuture.get() << '\n';



    

    return 0;
}