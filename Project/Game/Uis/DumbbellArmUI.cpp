#include "DumbbellArmUI.h"

#include "DirectXEngine.h"
#include "imgui.h"

#include <algorithm>
#include <cmath>

void DumbbellArmUI::Initialize()
{
    upperArmPosition_ = { 640.0f, 50.0f };
    foreArmPosition_ = { 19.0f, -2.5f };
    color_ = { 1.0f, 0.72f, 0.48f, 1.0f };
    anchorPoint_ = { 0.15f, 0.3f };

    upperArmLength_ = 228.0f;
    forearmLength_ = 252.0f;
    upperArmThickness_ = 120.0f;
    foreArmThickness_ = 120.0f;
    upperArmAngleDegrees_ = 90.0f;

    upperArmSprite_ = std::make_unique<Sprite>();
    upperArmSprite_->Initialize("upperArm.png");
    SetupArmSprite(upperArmSprite_.get());
    upperArmSprite_->SetSceneRenderer();

    forearmSprite_ = std::make_unique<Sprite>();
    forearmSprite_->Initialize("foreArm.png");
    SetupArmSprite(forearmSprite_.get());
    forearmSprite_->SetSceneRenderer();
}

void DumbbellArmUI::Finalize()
{
    if (DirectXEngine::GetSceneRenderer() != nullptr)
    {
        if (upperArmSprite_ != nullptr)
        {
            DirectXEngine::GetSceneRenderer()->SetRemoveList(upperArmSprite_.get());
        }

        if (forearmSprite_ != nullptr)
        {
            DirectXEngine::GetSceneRenderer()->SetRemoveList(forearmSprite_.get());
        }
    }

    upperArmSprite_.reset();
    forearmSprite_.reset();
}

void DumbbellArmUI::Update(float angleDegrees)
{
    if (upperArmSprite_ == nullptr || forearmSprite_ == nullptr)
    {
        return;
    }

    angleDegrees = std::clamp(angleDegrees, 0.0f, 160.0f);

    const float upperArmRadians = ToRadians(upperArmAngleDegrees_);
    const float forearmRadians = ToRadians(upperArmAngleDegrees_ + angleDegrees);

    UpdateArmSprite(
        upperArmSprite_.get(),
        upperArmPosition_,
        upperArmLength_,
        upperArmThickness_,
        upperArmRadians
    );

    const Vector2 elbowPosition = CalculateEndPosition(
        upperArmPosition_,
        upperArmLength_,
        upperArmRadians
    );

    forearmSprite_->SetAnchorPoint(anchorPoint_);
    UpdateArmSprite(
        forearmSprite_.get(),
        elbowPosition + foreArmPosition_,
        forearmLength_,
		foreArmThickness_,
        forearmRadians
    );
}

void DumbbellArmUI::SetArmLength(float upperArmLength, float forearmLength)
{
    upperArmLength_ = std::max(1.0f, upperArmLength);
    forearmLength_ = std::max(1.0f, forearmLength);
}

void DumbbellArmUI::SetupArmSprite(Sprite* sprite) const
{
    sprite->SetAnchorPoint({ 0.1f, 0.9f });
}

void DumbbellArmUI::UpdateArmSprite(Sprite* sprite, const Vector2& startPosition, 
    float length,float thickness, float rotateRadians) const
{
    sprite->GetTransform().position = startPosition;
    sprite->GetTransform().size = { length, thickness };
    sprite->GetTransform().rotate = rotateRadians;
    sprite->Update();
}

Vector2 DumbbellArmUI::CalculateEndPosition(const Vector2& startPosition, float length, float rotateRadians) const
{
    return {
        startPosition.x + std::cos(rotateRadians) * length,
        startPosition.y + std::sin(rotateRadians) * length
    };
}

float DumbbellArmUI::ToRadians(float degrees) const
{
    constexpr float pi = 3.14159265358979323846f;
    return degrees * pi / 180.0f;
}