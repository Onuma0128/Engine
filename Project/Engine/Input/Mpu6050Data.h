#pragma once

#include <string>
#include "Vector3.h"

struct Mpu6050Data
{
	Vector3 acceleration = { 0.0f, 0.0f, 0.0f };
	Vector3 gyro = { 0.0f, 0.0f, 0.0f };
};

bool ParseMpuData(
    const std::string& line,
    Mpu6050Data& data
);