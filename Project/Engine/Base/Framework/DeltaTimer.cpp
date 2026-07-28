#include "DeltaTimer.h"

// Static member definitions (C++14 compatible)
LARGE_INTEGER NumaEngine::DeltaTimer::frequency_ = {};
LARGE_INTEGER NumaEngine::DeltaTimer::startTime_ = {};
LARGE_INTEGER NumaEngine::DeltaTimer::prevTime_ = {};
float NumaEngine::DeltaTimer::deltaTime_ = 0.0f;
float NumaEngine::DeltaTimer::timeScale_ = 1.0f;
float NumaEngine::DeltaTimer::scaleTimer_ = 0.0f;

void NumaEngine::DeltaTimer::Initialize()
{
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&startTime_);
    prevTime_ = startTime_;
    timeScale_ = 1.0f;
    scaleTimer_ = 0.0f;
}

void NumaEngine::DeltaTimer::Update()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    // 秒単位のDeltaTime
    deltaTime_ = static_cast<float>(currentTime.QuadPart - prevTime_.QuadPart) / static_cast<float>(frequency_.QuadPart);

    prevTime_ = currentTime;

    if (scaleTimer_ > 0.0f) {
        scaleTimer_ -= deltaTime_;
        if (scaleTimer_ <= 0.0f) {        // ② 終了したら等速に戻す
            timeScale_ = 1.0f;
            scaleTimer_ = 0.0f;
        }
    }
}

void NumaEngine::DeltaTimer::SetTimeScaleForSeconds(float scale, float duration)
{
    timeScale_ = scale;
    scaleTimer_ = duration;
}
