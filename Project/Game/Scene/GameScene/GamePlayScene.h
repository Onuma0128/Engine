#pragma once

#include "BaseScene.h"
#include "SerialPort.h"
#include "Mpu6050Data.h"
#include "DumbbellCounter/DumbbellPoseCounter.h"

#include <string>

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public BaseScene
{
public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:

    /// <summary>
    /// MPU6050から届いたデータを更新する
    /// </summary>
    bool UpdateSensorData();

    /// <summary>
    /// ImGuiを表示する
    /// </summary>
    void DrawImGui();

    /// <summary>
    /// カウンター状態名を取得する
    /// </summary>
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
    float poseToleranceDegrees_ = 25.0f;

    // 姿勢範囲内に留まる必要がある時間
    float poseHoldTime_ = 0.10f;

    // この角速度以下なら静止とみなす
    float stableGyroThreshold_ = 35.0f;

    // ESP32が約50Hz、20ms間隔で送信する前提
    float sensorDeltaTime_ = 0.02f;

    // ==============================
    // デバッグ表示
    // ==============================

    bool showSensorWindow_ = true;

    float accelerationMagnitude_ = 0.0f;
};