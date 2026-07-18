#include "DumbbellPoseCounter.h"

#include <algorithm>

namespace
{
    constexpr float Pi = 3.14159265358979323846f;
    constexpr float MinimumLength = 0.0001f;
}

bool DumbbellPoseCounter::RecordExtendedPose(
    const Mpu6050Data& data)
{
    const Vector3 direction =
        GetGravityDirection(data);

    const float lengthSquared =
        direction.x * direction.x +
        direction.y * direction.y +
        direction.z * direction.z;

    if (lengthSquared < MinimumLength)
    {
        return false;
    }

    extendedPose_ = direction;
    hasExtendedPose_ = true;

    state_ = State::WaitingForExtended;
    poseTimer_ = 0.0f;

    return true;
}

bool DumbbellPoseCounter::RecordBentPose(
    const Mpu6050Data& data)
{
    const Vector3 direction =
        GetGravityDirection(data);

    const float lengthSquared =
        direction.x * direction.x +
        direction.y * direction.y +
        direction.z * direction.z;

    if (lengthSquared < MinimumLength)
    {
        return false;
    }

    bentPose_ = direction;
    hasBentPose_ = true;

    state_ = State::WaitingForExtended;
    poseTimer_ = 0.0f;

    return true;
}

void DumbbellPoseCounter::Update(
    const Mpu6050Data& data,
    float deltaTime)
{
    if (!IsCalibrationComplete())
    {
        poseTimer_ = 0.0f;
        return;
    }

    const Vector3 currentPose =
        GetGravityDirection(data);

    extendedAngle_ =
        CalculateAngleDegrees(currentPose, extendedPose_);

    bentAngle_ =
        CalculateAngleDegrees(currentPose, bentPose_);

    const float gyroMagnitude =
        CalculateGyroMagnitude(data);

    // 動作が激しい最中は、姿勢到達とみなさない
    const bool isStable =
        gyroMagnitude <= motionThreshold_;

    const bool isNearExtended =
        extendedAngle_ <= poseToleranceDegrees_;

    const bool isNearBent =
        bentAngle_ <= poseToleranceDegrees_;

    switch (state_)
    {
    case State::WaitingForExtended:

        if (isStable && isNearExtended)
        {
            poseTimer_ += deltaTime;

            if (poseTimer_ >= poseHoldTime_)
            {
                state_ = State::WaitingForBent;

                poseTimer_ = 0.0f;
                extendedPoseLocked_ = true;
                bentPoseLocked_ = false;
            }
        } else
        {
            poseTimer_ = 0.0f;
        }

        break;

    case State::WaitingForBent:

        // 一度伸ばした姿勢から十分離れるまで再判定を防ぐ
        if (extendedAngle_ >
            poseToleranceDegrees_ * 1.5f)
        {
            extendedPoseLocked_ = false;
        }

        if (!extendedPoseLocked_ &&
            isStable &&
            isNearBent)
        {
            poseTimer_ += deltaTime;

            if (poseTimer_ >= poseHoldTime_)
            {
                state_ = State::WaitingForReturn;

                poseTimer_ = 0.0f;
                bentPoseLocked_ = true;
            }
        } else
        {
            poseTimer_ = 0.0f;
        }

        break;

    case State::WaitingForReturn:

        // 一度曲げた姿勢から離れた後に伸ばし姿勢を許可
        if (bentAngle_ >
            poseToleranceDegrees_ * 1.5f)
        {
            bentPoseLocked_ = false;
        }

        if (!bentPoseLocked_ &&
            isStable &&
            isNearExtended)
        {
            poseTimer_ += deltaTime;

            if (poseTimer_ >= poseHoldTime_)
            {
                ++count_;

                state_ = State::WaitingForBent;

                poseTimer_ = 0.0f;
                extendedPoseLocked_ = true;
                bentPoseLocked_ = false;
            }
        } else
        {
            poseTimer_ = 0.0f;
        }

        break;
    }
}

void DumbbellPoseCounter::ResetCount()
{
    count_ = 0;
    state_ = State::WaitingForExtended;

    poseTimer_ = 0.0f;
    extendedPoseLocked_ = false;
    bentPoseLocked_ = false;
}

void DumbbellPoseCounter::ResetCalibration()
{
    hasExtendedPose_ = false;
    hasBentPose_ = false;

    extendedPose_ = {};
    bentPose_ = {};

    extendedAngle_ = 180.0f;
    bentAngle_ = 180.0f;

    ResetCount();
}

int DumbbellPoseCounter::GetCount() const
{
    return count_;
}

DumbbellPoseCounter::State
DumbbellPoseCounter::GetState() const
{
    return state_;
}

bool DumbbellPoseCounter::HasExtendedPose() const
{
    return hasExtendedPose_;
}

bool DumbbellPoseCounter::HasBentPose() const
{
    return hasBentPose_;
}

bool DumbbellPoseCounter::IsCalibrationComplete() const
{
    return hasExtendedPose_ && hasBentPose_;
}

float DumbbellPoseCounter::GetExtendedAngle() const
{
    return extendedAngle_;
}

float DumbbellPoseCounter::GetBentAngle() const
{
    return bentAngle_;
}

void DumbbellPoseCounter::SetPoseToleranceDegrees(
    float degrees)
{
    poseToleranceDegrees_ =
        std::clamp(degrees, 1.0f, 60.0f);
}

void DumbbellPoseCounter::SetPoseHoldTime(
    float seconds)
{
    poseHoldTime_ =
        std::clamp(seconds, 0.05f, 2.0f);
}

void DumbbellPoseCounter::SetMotionThreshold(
    float degreePerSecond)
{
    motionThreshold_ =
        std::clamp(degreePerSecond, 1.0f, 250.0f);
}

float DumbbellPoseCounter::GetPoseToleranceDegrees() const
{
    return poseToleranceDegrees_;
}

float DumbbellPoseCounter::GetPoseHoldTime() const
{
    return poseHoldTime_;
}

float DumbbellPoseCounter::GetMotionThreshold() const
{
    return motionThreshold_;
}

Vector3 DumbbellPoseCounter::GetGravityDirection(
    const Mpu6050Data& data) const
{
    Vector3 result{
        data.acceleration.x,
        data.acceleration.y,
        data.acceleration.z
    };

    const float length = std::sqrt(
        result.x * result.x +
        result.y * result.y +
        result.z * result.z
    );

    if (length < MinimumLength)
    {
        return {};
    }

    result.x /= length;
    result.y /= length;
    result.z /= length;

    return result;
}

float DumbbellPoseCounter::CalculateAngleDegrees(
    const Vector3& a,
    const Vector3& b) const
{
    float dot =
        a.x * b.x +
        a.y * b.y +
        a.z * b.z;

    dot = std::clamp(dot, -1.0f, 1.0f);

    return std::acos(dot) * 180.0f / Pi;
}

float DumbbellPoseCounter::CalculateGyroMagnitude(
    const Mpu6050Data& data) const
{
    return std::sqrt(
        data.gyro.x * data.gyro.x +
        data.gyro.y * data.gyro.y +
        data.gyro.z * data.gyro.z
    );
}