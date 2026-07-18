#pragma once

#include "Mpu6050Data.h"
#include "Vector3.h"

#include <cmath>

class DumbbellPoseCounter
{
public:
    enum class State
    {
		WaitingForExtended,     // 腕を伸ばした姿勢を待っている
		WaitingForBent,         // 腕を曲げた姿勢を待っている
		WaitingForReturn        // 腕を伸ばした姿勢に戻るのを待っている
    };

public:
    // 現在の姿勢を「腕を伸ばした状態」として保存
    bool RecordExtendedPose(const Mpu6050Data& data);

    // 現在の姿勢を「腕を曲げた状態」として保存
    bool RecordBentPose(const Mpu6050Data& data);

    void Update(
        const Mpu6050Data& data,
        float deltaTime
    );

    void ResetCount();
    void ResetCalibration();

    int GetCount() const;
    State GetState() const;

    bool HasExtendedPose() const;
    bool HasBentPose() const;
    bool IsCalibrationComplete() const;

    float GetExtendedAngle() const;
    float GetBentAngle() const;

    void SetPoseToleranceDegrees(float degrees);
    void SetPoseHoldTime(float seconds);
    void SetMotionThreshold(float degreePerSecond);

    float GetPoseToleranceDegrees() const;
    float GetPoseHoldTime() const;
    float GetMotionThreshold() const;

private:
    Vector3 GetGravityDirection(
        const Mpu6050Data& data
    ) const;

    float CalculateAngleDegrees(
        const Vector3& a,
        const Vector3& b
    ) const;

    float CalculateGyroMagnitude(
        const Mpu6050Data& data
    ) const;

private:
    Vector3 extendedPose_{};
    Vector3 bentPose_{};

    bool hasExtendedPose_ = false;
    bool hasBentPose_ = false;

    State state_ = State::WaitingForExtended;

    int count_ = 0;

    float extendedAngle_ = 180.0f;
    float bentAngle_ = 180.0f;

    // 記録姿勢から何度までずれてよいか
    float poseToleranceDegrees_ = 18.0f;

    // 姿勢範囲内に何秒いれば到達と認めるか
    float poseHoldTime_ = 0.25f;

    // この角速度以下なら「姿勢が安定している」とみなす
    float motionThreshold_ = 25.0f;

    float poseTimer_ = 0.0f;

    // 同じ姿勢を連続判定しないためのロック
    bool extendedPoseLocked_ = false;
    bool bentPoseLocked_ = false;
};