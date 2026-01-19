#include "BossHpUI.h"

#include "Objects/Boss/Base/BossEnemy.h"

void BossHpBarUI::Init(const Vector2& position)
{
	Sprite::Initialize("bossHpBar.png");

	transform_.position = position;
	preTextureSize_ = textureSize_.x;
}

void BossHpBarUI::Update(const Vector2& position)
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetMainData();
	uint32_t maxHp = boss_->GetMaxHP();
	uint32_t currentHp = boss_->GetCurrentHp();
	float scale = static_cast<float>(currentHp) / static_cast<float>(maxHp);
	// ボスのHPに応じて色を変える
	Vector4 color = { 1.0f - scale,scale,0.0f,1.0f };
	Sprite::SetColor(color);
	// ボスのHPに応じてスケールを縮める
	textureSize_.x = preTextureSize_ * scale;
	transform_.size.x = preTextureSize_ * scale;
	transform_.position = position;

	Sprite::Update();
}

void BossHpBarUI::Draw()
{
	Sprite::Draw();
}