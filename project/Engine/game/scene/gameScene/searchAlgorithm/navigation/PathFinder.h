#pragma once
#include <memory>
#include <vector>

#define NOMINMAX

#include "Vector2.h"
#include "Vector3.h"

#include "gameScene/searchAlgorithm/movement/SplineMover.h"

class MapCollision;

class PathFinder
{
public:

	// 探索をしてSplineを再初期化
	void Search(const Vector3& startW, const Vector3& goalW);
	// 時間を進めて座標を更新する
	void Update(const float speed, float lookAt_t = 0.01f);

	// 今の座標と次の座標を取得
	const Vector3& GetVelocity() { return splineMover_.GetVelocity(); }
	const Vector3& GetPosition() { return splineMover_.GetPosition(); }
	const Quaternion& GetRotation() { return splineMover_.GetRotation(); }
	void DebugSpline() { splineMover_.DebugSpline(); }

	// マップデータを取得する
	void SetMapData(MapCollision* mapColl) { mapColl_ = mapColl; }

private:

	// グリッドのIndex座標
	struct GridPos { uint32_t x, z; };

	// 再帰ノード
	struct A_StarNode {
		GridPos pos;				// 座標(グリッドのIndex番号)
		uint32_t actualCost;		// 実コスト
		uint32_t estimatedCost;		// 推定コスト
		uint32_t score;				// スコア(実コスト + 推定コスト)
		A_StarNode* parent;			// 親のノード
	};

	// 判定を取ったマップを持つ
	MapCollision* mapColl_ = nullptr;

	// スプライン上の動き
	SplineMover splineMover_;
};

