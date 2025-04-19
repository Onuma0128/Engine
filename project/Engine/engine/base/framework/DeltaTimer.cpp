#include "DeltaTimer.h"

void DeltaTimer::Initialize()
{
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&startTime_);
    prevTime_ = startTime_;
}

void DeltaTimer::Update()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    // 秒単位のDeltaTime
    deltaTime_ = static_cast<float>(currentTime.QuadPart - prevTime_.QuadPart) / static_cast<float>(frequency_.QuadPart);

    prevTime_ = currentTime;
}
