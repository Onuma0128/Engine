
#pragma once

#include "Sprite.h"

// 前方宣言
class BossEnemy;

class BossHpBarUI : public NumaEngine::Sprite
{
public:
    void Init(const NumaEngine::Vector2& position);
    void Update(const NumaEngine::Vector2& position);
    void Draw();

    void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }

private:
    BossEnemy* boss_ = nullptr;
    float preTextureSize_ = 0.0f;
};

