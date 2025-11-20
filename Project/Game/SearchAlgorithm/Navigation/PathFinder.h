#pragma once
#include <memory>
#include <vector>

#define NOMINMAX

#include "Vector2.h"
#include "Vector3.h"

#include "searchAlgorithm/movement/SplineMover.h"

class MapCollision;

/// <summary>
/// 探索アルゴリズムの経路探索を管理するクラス
/// </summary>
class PathFinder
{
public:

	/// <summary>
	/// スプラインをリセットする
	/// </summary>
	void Reset();

	/// <summary>
	/// 探索をしてSplineを再初期化
	/// </summary>
	/// <param name="startW"></スタートの座標>
	/// <param name="goalW"></ゴールの座標>
	void Search(const Vector3& startW, const Vector3& goalW);

	/// <summary>
	/// 時間を進めて座標を更新
	/// </summary>
	/// <param name="speed"></スプラインを進むスピードを設定する>
	/// <param name="lookAt_t"></0 ～ 1のtimeを入れる>
	void Update(const float speed, float lookAt_t = 0.01f);

	// 今の座標と次の座標を取得

	/// <summary>
	/// 現在のスプライン上の速度を取得する
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() { return splineMover_.GetVelocity(); }

	/// <summary>
	/// 現在のスプライン上の座標を取得する
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() { return splineMover_.GetPosition(); }

	/// <summary>
	/// 現在のスプライン上のy回転のクォータニオンを取得する
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() { return splineMover_.GetRotation(); }

	/// <summary>
	/// スプラインのデバッグ描画を有効/無効にする
	/// </summary>
	/// <param name="flag"></param>
	void DebugSpline(bool flag) { splineMover_.DebugSpline(flag); }

	/// <summary>
	/// マップデータを設定する
	/// </summary>
	/// <param name="mapColl"></param>
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

