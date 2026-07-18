#include "Mpu6050Data.h"

#include <sstream>
#include <string>
#include <vector>

bool ParseMpuData(
    const std::string& line,
    Mpu6050Data& data)
{
    std::stringstream stream(line);
    std::string value;
    std::vector<std::string> values;

    while (std::getline(stream, value, ','))
    {
        values.push_back(value);
    }

    if (values.size() != 6)
    {
        return false;
    }

    try
    {
        data.acceleration.x = std::stof(values[0]);
        data.acceleration.y = std::stof(values[1]);
        data.acceleration.z = std::stof(values[2]);

        data.gyro.x = std::stof(values[3]);
        data.gyro.y = std::stof(values[4]);
        data.gyro.z = std::stof(values[5]);
    }
    catch (...)
    {
        return false;
    }

    return true;
}