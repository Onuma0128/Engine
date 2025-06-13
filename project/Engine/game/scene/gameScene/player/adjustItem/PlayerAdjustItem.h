#pragma once

#include "JsonFunction.h"

struct BulletUIData {
	Vector2 size;
	Vector2 position;
	float startPosition;
};

class PlayerAdjustItem
{
public:

	void LoadItems();

	void Editor();

	BulletUIData GetBulletUIData()const { return bulletUIData_; }


private:

	// 弾UIの項目
	BulletUIData bulletUIData_;
	JsonFunction bulletUIJson_;

};

