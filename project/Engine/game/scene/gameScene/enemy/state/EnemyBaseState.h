#pragma once

class BaseEnemy;

class EnemyBaseState
{
public:

	~EnemyBaseState() = default;
	EnemyBaseState(BaseEnemy* enemy);

	virtual void Init() = 0;

	virtual void Finalize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

protected:

	BaseEnemy* enemy_ = nullptr;

};
