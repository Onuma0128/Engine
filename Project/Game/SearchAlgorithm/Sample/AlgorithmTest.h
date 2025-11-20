#pragma once

#include <memory>
#include <vector>

#include "Object3d.h"
#include "Vector3.h"

#include "searchAlgorithm/navigation/PathFinder.h"

class MapCollision;

/// <summary>
/// アルゴリズムのテストクラス
/// </summary>
class AlgorithmTest : public Object3d
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="goal"></param>
	void Update(const Vector3& goal);

	/// <summary>
	/// マップコリジョンを設定する
	/// </summary>
	/// <param name="mapCollision"></param>
	void SetMapCollision(MapCollision* mapCollision) { pathFinder_.SetMapData(mapCollision); }

private:

	/// <summary>
	/// リセット
	/// </summary>
	/// <param name="goal"></param>
	void Reset(const Vector3& goal);

private:

	// 探索アルゴリズム
	PathFinder pathFinder_;

	float isSearchTime_ = 0.0f;
	float searchDecisionTime_ = 1.0f;

	float speed_;
	float lerpSpeed_;
	bool drawSpline_ = false;
};

