#pragma once
#include <memory>

#include "Collider.h"
#include "Sprite.h"

class PlayerReticle :public Sprite,Collider
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

private:

	void SegmentUpdate();

};

