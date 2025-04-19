#pragma once

#include <Windows.h>

class DeltaTimer
{
public:

    static void Initialize();

    static void Update();

    static float GetDeltaTime() { return deltaTime_; }

private:
    static inline LARGE_INTEGER frequency_;
    static inline LARGE_INTEGER startTime_;
    static inline LARGE_INTEGER prevTime_;
    static inline float deltaTime_ = 0.0f;
};
