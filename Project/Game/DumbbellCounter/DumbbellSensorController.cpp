#include "DumbbellSensorController.h"

#include "imgui.h"

#include <Windows.h>
#include <cmath>
#include <string>

#include "DirectXEngine.h"
#include "Input.h"

void DumbbellSensorController::Initialize()
{
    // COM4を開く
    isSerialConnected_ =
        serialPort_.Open(
            serialPortName_,
            CBR_115200
        );

    if (isSerialConnected_)
    {
        OutputDebugStringA(
            "COM4 opened successfully.\n"
        );
    } else
    {
        OutputDebugStringA(
            "Failed to open COM4.\n"
        );
    }

    mpuData_ = {};

    lastReceivedLine_.clear();

    parseSucceeded_ = false;
    receivedNewSensorData_ = false;

    dumbbellPoseCounter_.ResetCalibration();

    previousDumbbellCount_ = 0;

    accelerationMagnitude_ = 0.0f;

    framesSinceLastSensorData_ = communicationTimeoutFrames_;

    connectionStateSprite_ = std::make_unique<Sprite>();
    connectionStateSprite_->Initialize("white1x1.png");
    connectionStateSprite_->SetAnchorPoint({ 0.5f, 0.5f });
    connectionStateSprite_->GetTransform().position = connectionStateSpritePosition_;
    connectionStateSprite_->GetTransform().size = connectionStateSpriteSize_;
    UpdateConnectionSprite();
    connectionStateSprite_->SetSceneRenderer();

    InitializePoseRecordStateSprite(
        extendedPoseRecordStateSprite_,
        extendedPoseRecordStateSpritePosition_
    );
    InitializePoseRecordStateSprite(
        bentPoseRecordStateSprite_,
        bentPoseRecordStateSpritePosition_
    );
    UpdatePoseRecordStateSprites();
}

void DumbbellSensorController::Finalize()
{
    if (DirectXEngine::GetSceneRenderer() != nullptr &&
        connectionStateSprite_ != nullptr)
    {
        DirectXEngine::GetSceneRenderer()->SetRemoveList(
            connectionStateSprite_.get()
        );
    }

    if (DirectXEngine::GetSceneRenderer() != nullptr &&
        extendedPoseRecordStateSprite_ != nullptr)
    {
        DirectXEngine::GetSceneRenderer()->SetRemoveList(
            extendedPoseRecordStateSprite_.get()
        );
    }

    if (DirectXEngine::GetSceneRenderer() != nullptr &&
        bentPoseRecordStateSprite_ != nullptr)
    {
        DirectXEngine::GetSceneRenderer()->SetRemoveList(
            bentPoseRecordStateSprite_.get()
        );
    }

    connectionStateSprite_.reset();
    extendedPoseRecordStateSprite_.reset();
    bentPoseRecordStateSprite_.reset();

    serialPort_.Close();

    isSerialConnected_ = false;
}

void DumbbellSensorController::Update()
{
    dumbbellPoseCounter_.SetPoseToleranceDegrees(
        poseToleranceDegrees_
    );

    dumbbellPoseCounter_.SetPoseHoldTime(
        poseHoldTime_
    );

    dumbbellPoseCounter_.SetMotionThreshold(
        stableGyroThreshold_
    );

    receivedNewSensorData_ = UpdateSensorData();

    if (receivedNewSensorData_)
    {
        framesSinceLastSensorData_ = 0;
    } else if (framesSinceLastSensorData_ < communicationTimeoutFrames_)
    {
        ++framesSinceLastSensorData_;
    }

    UpdateConnectionSprite();

    Input* input = Input::GetInstance();

    // 腕を伸ばした状態を記録
    if (input->TriggerKey(DIK_DOWN))
    {
        const bool recorded =
            dumbbellPoseCounter_.RecordExtendedPose(mpuData_);

        OutputDebugStringA(
            recorded
            ? "Extended pose recorded.\n"
            : "Failed to record extended pose.\n"
        );
    }

    // 腕を曲げた状態を記録
    if (input->TriggerKey(DIK_UP))
    {
        const bool recorded =
            dumbbellPoseCounter_.RecordBentPose(mpuData_);

        OutputDebugStringA(
            recorded
            ? "Bent pose recorded.\n"
            : "Failed to record bent pose.\n"
        );
    }

    if (input->TriggerKey(DIK_RETURN))
    {
        dumbbellPoseCounter_.ResetCalibration();
        dumbbellPoseCounter_.ResetCount();

        previousDumbbellCount_ = 0;
    }

    // 入力による記録・リセット後に更新する
    UpdatePoseRecordStateSprites();

    accelerationMagnitude_ = std::sqrt(
        mpuData_.acceleration.x * mpuData_.acceleration.x +
        mpuData_.acceleration.y * mpuData_.acceleration.y +
        mpuData_.acceleration.z * mpuData_.acceleration.z
    );

    const int currentCount =
        dumbbellPoseCounter_.GetCount();

    if (currentCount > previousDumbbellCount_)
    {
        OutputDebugStringA("Dumbbell Count +1\n");
        previousDumbbellCount_ = currentCount;
    }
}

bool DumbbellSensorController::UpdateSensorData()
{
    if (!serialPort_.IsOpen())
    {
        isSerialConnected_ = false;
        return false;
    }

    isSerialConnected_ = true;

    std::string line;
    bool receivedNewData = false;

    while (serialPort_.ReadLine(line))
    {
        lastReceivedLine_ = line;

        Mpu6050Data newData{};

        if (ParseMpuData(line, newData))
        {
            mpuData_ = newData;

            parseSucceeded_ = true;
            receivedNewData = true;

            // 受信した1サンプルごとに判定する
            dumbbellPoseCounter_.Update(
                newData,
                sensorDeltaTime_
            );
        } else
        {
            parseSucceeded_ = false;
        }
    }

    return receivedNewData;
}

void DumbbellSensorController::UpdateConnectionSprite()
{
    if (connectionStateSprite_ == nullptr)
    {
        return;
    }

    connectionStateSprite_->GetTransform().position =
        connectionStateSpritePosition_;
    connectionStateSprite_->GetTransform().size =
        connectionStateSpriteSize_;

    connectionStateSprite_->SetColor(
        IsSensorCommunicating()
        ? Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }
        : Vector4{ 1.0f, 0.0f, 0.0f, 1.0f }
    );

    connectionStateSprite_->Update();
}

void DumbbellSensorController::InitializePoseRecordStateSprite(
    std::unique_ptr<Sprite>& sprite,
    const Vector2& position)
{
    sprite = std::make_unique<Sprite>();
    sprite->Initialize("settingSwitchUI.png");

    sprite->SetAnchorPoint({ 0.5f, 0.5f });

    sprite->GetTransform().position = position;
    sprite->GetTransform().size = poseRecordStateSpriteSize_;

    // 切り出す1コマのサイズ
    sprite->SetTextureSize({
        kPoseRecordStateTextureWidth,
        kPoseRecordStateTextureHeight
        });

    // 最初はNOを表示
    sprite->SetTextureLeftTop({
        kPoseRecordStateNoTextureLeft,
        0.0f
        });

    // 初期状態を反映
    sprite->Update();
    sprite->SetSceneRenderer();
}

void DumbbellSensorController::UpdatePoseRecordStateSprite(
    Sprite* sprite,
    bool isRecorded) const
{
    if (sprite == nullptr)
    {
        return;
    }

    const float textureLeft =
        isRecorded
        ? kPoseRecordStateOkTextureLeft
        : kPoseRecordStateNoTextureLeft;

    sprite->SetTextureLeftTop({
        textureLeft,
        0.0f
        });

    // SetTextureLeftTopの変更を描画へ反映する
    sprite->Update();
}

void DumbbellSensorController::UpdatePoseRecordStateSprites()
{
    UpdatePoseRecordStateSprite(
        extendedPoseRecordStateSprite_.get(),
        dumbbellPoseCounter_.HasExtendedPose()
    );

    UpdatePoseRecordStateSprite(
        bentPoseRecordStateSprite_.get(),
        dumbbellPoseCounter_.HasBentPose()
    );
}

bool DumbbellSensorController::IsSensorCommunicating() const
{
    return isSerialConnected_ &&
        parseSucceeded_ &&
        framesSinceLastSensorData_ < communicationTimeoutFrames_;
}

void DumbbellSensorController::DrawImGui()
{
    if (!showSensorWindow_)
    {
        return;
    }

    ImGui::Begin(
        "MPU6050 Dumbbell Monitor",
        &showSensorWindow_
    );

    // ====================================
    // 接続状態
    // ====================================

    ImGui::SeparatorText("Connection");

    ImGui::Text(
        "Serial Port: %s",
        serialPortName_.c_str()
    );

    ImGui::Text(
        "Connection: %s",
        isSerialConnected_
        ? "Connected"
        : "Disconnected"
    );

    ImGui::Text(
        "Parse: %s",
        parseSucceeded_
        ? "Success"
        : "Failed"
    );

    ImGui::Text(
        "New Data: %s",
        receivedNewSensorData_
        ? "Yes"
        : "No"
    );

    // ====================================
    // 加速度
    // ====================================

    ImGui::SeparatorText("Acceleration");

    ImGui::Text(
        "X: %.3f g",
        mpuData_.acceleration.x
    );

    ImGui::Text(
        "Y: %.3f g",
        mpuData_.acceleration.y
    );

    ImGui::Text(
        "Z: %.3f g",
        mpuData_.acceleration.z
    );

    ImGui::Text(
        "Magnitude: %.3f g",
        accelerationMagnitude_
    );

    // ====================================
    // ジャイロ
    // ====================================

    ImGui::SeparatorText("Gyroscope");

    ImGui::Text(
        "X: %.3f deg/s",
        mpuData_.gyro.x
    );

    ImGui::Text(
        "Y: %.3f deg/s",
        mpuData_.gyro.y
    );

    ImGui::Text(
        "Z: %.3f deg/s",
        mpuData_.gyro.z
    );

    // ====================================
    // 姿勢登録
    // ====================================

    ImGui::SeparatorText("Pose Calibration");

    ImGui::Text(
        "Extended Pose: %s",
        dumbbellPoseCounter_.HasExtendedPose()
        ? "Recorded"
        : "Not Recorded"
    );

    ImGui::Text(
        "Bent Pose: %s",
        dumbbellPoseCounter_.HasBentPose()
        ? "Recorded"
        : "Not Recorded"
    );

    ImGui::Text(
        "Calibration: %s",
        dumbbellPoseCounter_.IsCalibrationComplete()
        ? "Complete"
        : "Incomplete"
    );

    ImGui::TextWrapped(
        "Keep your arm still in the desired position, "
        "then press the corresponding record button."
    );

    if (ImGui::Button("Record Extended Pose"))
    {
        const bool recorded =
            dumbbellPoseCounter_.RecordExtendedPose(
                mpuData_
            );

        if (recorded)
        {
            OutputDebugStringA(
                "Extended pose recorded.\n"
            );
        } else
        {
            OutputDebugStringA(
                "Failed to record extended pose.\n"
            );
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Record Bent Pose"))
    {
        const bool recorded =
            dumbbellPoseCounter_.RecordBentPose(
                mpuData_
            );

        if (recorded)
        {
            OutputDebugStringA(
                "Bent pose recorded.\n"
            );
        } else
        {
            OutputDebugStringA(
                "Failed to record bent pose.\n"
            );
        }
    }

    if (ImGui::Button("Reset Calibration"))
    {
        dumbbellPoseCounter_.ResetCalibration();

        previousDumbbellCount_ = 0;
    }

    // ====================================
    // カウント判定
    // ====================================

    ImGui::SeparatorText("Dumbbell Counter");

    ImGui::Text(
        "Count: %d",
        dumbbellPoseCounter_.GetCount()
    );

    ImGui::Text(
        "State: %s",
        GetPoseStateName(
            dumbbellPoseCounter_.GetState()
        )
    );

    ImGui::Text(
        "Extended Angle: %.2f deg",
        dumbbellPoseCounter_.GetExtendedAngle()
    );

    ImGui::Text(
        "Bent Angle: %.2f deg",
        dumbbellPoseCounter_.GetBentAngle()
    );

    if (ImGui::Button("Reset Count"))
    {
        dumbbellPoseCounter_.ResetCount();

        previousDumbbellCount_ = 0;
    }

    // ====================================
    // 判定設定
    // ====================================

    ImGui::SeparatorText("Detection Settings");

    ImGui::DragFloat(
        "Pose Tolerance",
        &poseToleranceDegrees_,
        0.5f,
        5.0f,
        60.0f,
        "%.1f deg"
    );

    ImGui::DragFloat(
        "Pose Hold Time",
        &poseHoldTime_,
        0.01f,
        0.05f,
        1.0f,
        "%.2f sec"
    );

    ImGui::DragFloat(
        "Stable Gyro Threshold",
        &stableGyroThreshold_,
        1.0f,
        5.0f,
        150.0f,
        "%.1f deg/s"
    );

    ImGui::DragFloat(
        "Sensor Delta Time",
        &sensorDeltaTime_,
        0.001f,
        0.005f,
        0.1f,
        "%.3f sec"
    );

    ImGui::TextWrapped(
        "If detection is too strict, increase Pose Tolerance. "
        "If poses are detected too easily, increase Pose Hold Time."
    );

    // ====================================
    // 生データ
    // ====================================

    ImGui::SeparatorText("Raw Serial Data");

    if (lastReceivedLine_.empty())
    {
        ImGui::TextDisabled(
            "No serial data received."
        );
    } else
    {
        ImGui::TextWrapped(
            "%s",
            lastReceivedLine_.c_str()
        );
    }

    ImGui::End();
}

const char* DumbbellSensorController::GetPoseStateName(
    DumbbellPoseCounter::State state) const
{
    switch (state)
    {
    case DumbbellPoseCounter::State::WaitingForExtended:

        return "Waiting For Extended";

    case DumbbellPoseCounter::State::WaitingForBent:

        return "Waiting For Bent";

    case DumbbellPoseCounter::State::WaitingForReturn:

        return "Waiting For Return";

    default:

        return "Unknown";
    }
}
const Mpu6050Data& DumbbellSensorController::GetMpuData() const
{
    return mpuData_;
}

float DumbbellSensorController::GetExtendedAngle() const
{
    return dumbbellPoseCounter_.GetExtendedAngle();
}

float DumbbellSensorController::GetBentAngle() const
{
    return dumbbellPoseCounter_.GetBentAngle();
}

int DumbbellSensorController::GetDumbbellCount() const
{
    return dumbbellPoseCounter_.GetCount();
}

DumbbellPoseCounter::State DumbbellSensorController::GetDumbbellState() const
{
	return dumbbellPoseCounter_.GetState();
}
