#pragma once

#include <memory>
#include <vector>

#include "Object3d.h"
#include "Vector3.h"

#include "gameScene/searchAlgorithm/navigation/PathFinder.h"

class MapCollision;

class AlgorithmTest : public Object3d
{
public:

	void Init();

	void Update(const Vector3& goal);

	void SetMapCollision(MapCollision* mapCollision) { pathFinder_.SetMapData(mapCollision); }

private:

	void Reset(const Vector3& goal);

private:

	// 探索アルゴリズム
	PathFinder pathFinder_;

	float speed_;
	float lerpSpeed_;
	bool drawSpline_ = false;
};

