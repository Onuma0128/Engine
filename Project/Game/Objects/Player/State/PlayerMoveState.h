
#pragma once

#include "PlayerBaseState.h"
#include "Vector3.h"
#include "Quaternion.h"

class Player;

class PlayerMoveState : public PlayerBaseState {
public:
    PlayerMoveState(Player* player);

    PlayerState GetState() const override { return PlayerState::Move; }
    void Init() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;

private:
    void SomeAction();
    const NumaEngine::Vector3 CreateMoveVelocity();
    const NumaEngine::Vector3 CreateRotateVelocity();

private:
    bool isReversePlay_ = false;
    bool chengeAniamtion_ = false; // original code has typo

    NumaEngine::Quaternion rightStickQuaternion_{};
    NumaEngine::Vector3 rightStickVelocity_{};
};

