#pragma once

#include <Windows.h>

/// <summary>
/// 時間管理クラス
/// </summary>
class DeltaTimer
{
public:

	// 初期化
    static void Initialize();
	// 更新
    static void Update();
	// デルタタイムの取得
    static float GetDeltaTime() { 
        if (deltaTime_ >= 1.0f) { return 0.0f; }
        return deltaTime_ * timeScale_;
    }
	// 生のデルタタイムの取得
    static float GetRawDeltaTime() { 
        if (deltaTime_ >= 1.0f) { return 0.0f; }
        return deltaTime_; 
    }
	// 経過時間の取得
    static void  SetTimeScaleForSeconds(float scale, float duration);

private:
    static inline LARGE_INTEGER frequency_;
    static inline LARGE_INTEGER startTime_;
    static inline LARGE_INTEGER prevTime_;
    static inline float deltaTime_ = 0.0f;      // 今の時間
    static inline float  timeScale_ = 1.0f;     // 時間のスケール
    static inline float  scaleTimer_ = 0.0f;    // 時間のスケールをしている秒数
};
