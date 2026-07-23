#pragma once

#include "DumbbellCounter/DumbbellPoseCounter.h"
#include "Mpu6050Data.h"
#include "SerialPort.h"
#include "Sprite.h"

#include <memory>
#include <string>

class DumbbellSensorController
{
public:
    void Initialize();
    void Finalize();
    void Update();
    void DrawImGui();

    const Mpu6050Data& GetMpuData() const;
    float GetExtendedAngle() const;
    float GetBentAngle() const;
    int GetDumbbellCount() const;
    bool IsCalibrationComplete() const;
	DumbbellPoseCounter::State GetDumbbellState() const;

private:
    bool UpdateSensorData();
    void UpdateConnectionSprite();
    void InitializePoseRecordStateSprite(
        std::unique_ptr<Sprite>& sprite,
        const Vector2& position
    );
    void UpdatePoseRecordStateSprite(
        Sprite* sprite,
        bool isRecorded
    ) const;
    void UpdatePoseRecordStateSprites();
    bool IsSensorCommunicating() const;

    const char* GetPoseStateName(
        DumbbellPoseCounter::State state
    ) const;

private:

    // ==============================
    // シリアル通信
    // ==============================

    SerialPort serialPort_;

    std::string serialPortName_ = "COM4";

    bool isSerialConnected_ = false;

    // 最後に受信した文字列
    std::string lastReceivedLine_;

    // CSV解析が成功したか
    bool parseSucceeded_ = false;

    // 今フレームでセンサー値を受信したか
    bool receivedNewSensorData_ = false;

    // センサー値を最後に受信してからのフレーム数
    int framesSinceLastSensorData_ = 0;

    // このフレーム数以内に受信していれば通信中とみなす
    int communicationTimeoutFrames_ = 30;

    // ==============================
    // MPU6050
    // ==============================

    Mpu6050Data mpuData_{};

    // ==============================
    // ダンベルカウンター
    // ==============================

    DumbbellPoseCounter dumbbellPoseCounter_;

    // 前フレームのカウント
    int previousDumbbellCount_ = 0;

    // 記録姿勢からの許容角度
    float poseToleranceDegrees_ = 40.0f;

    // 姿勢範囲内に留まる必要がある時間
    float poseHoldTime_ = 0.01f;

    // この角速度以下なら静止とみなす
    float stableGyroThreshold_ = 40.0f;

    // ESP32が約50Hz、20ms間隔で送信する前提
    float sensorDeltaTime_ = 0.02f;

    // ==============================
    // デバッグ表示
    // ==============================

    bool showSensorWindow_ = true;

    float accelerationMagnitude_ = 0.0f;

    // ==============================
    // 通信状態表示
    // ==============================

    std::unique_ptr<Sprite> connectionStateSprite_;

    Vector2 connectionStateSpritePosition_ = { 16.0f, 704.0f };
    Vector2 connectionStateSpriteSize_ = { 32.0f, 32.0f };

    // ==============================
   // 姿勢記録状態表示
   // ==============================

    std::unique_ptr<Sprite> extendedPoseRecordStateSprite_;
    std::unique_ptr<Sprite> bentPoseRecordStateSprite_;

    Vector2 extendedPoseRecordStateSpritePosition_ = { 150.0f, 392.0f };
    Vector2 bentPoseRecordStateSpritePosition_ = { 150.0f,328.0f };
    Vector2 poseRecordStateSpriteSize_ = { 128.0f, 64.0f };

    static constexpr float kPoseRecordStateNoTextureLeft = 0.0f;
    static constexpr float kPoseRecordStateOkTextureLeft = 128.0f;
    static constexpr float kPoseRecordStateTextureWidth = 128.0f;
    static constexpr float kPoseRecordStateTextureHeight = 64.0f;
};