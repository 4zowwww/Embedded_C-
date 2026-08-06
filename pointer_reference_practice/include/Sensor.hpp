#pragma once

class Sensor
{
public:
    explicit Sensor(int id);
    
    ~Sensor();

    void read() const;

private:
    int id_;
};

