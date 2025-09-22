#pragma once

#include <memory>

#include "BaseEnemy.h"

#include "../bullet/EnemyBullet.h"

class RangedEliteEnemy : public BaseEnemy
{
public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Dead() override;
	void Reset(const Vector3& position) override;

	std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() override { return bullets_; }

private:

	std::vector<std::unique_ptr<EnemyBullet>> bullets_;
};

