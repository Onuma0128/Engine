#pragma once
#include <memory>
#include <vector>

#include "Line3d.h"
#include "Collision2D.h"

/// <summary>
/// マップのグリッドを描画するクラス
/// </summary>
class DrawGrid
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="cell"></グリッド1マス分の大きさ>
	/// <param name="size"></全体の大きさ>
	void Init(const float cell, const float size);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// グリッドの描画を設定する
	/// </summary>
	/// <param name="flag"></描画するかをフラグ管理>
	void SetDrawGrid(bool flag);
	
	/// <summary>
	/// グリッドの位置を設定する
	/// </summary>
	/// <param name="cell"></グリッド1マス分の大きさ>
	/// <param name="size"></全体の大きさ>
	void SetGridPositions(const float cell, const float size);

	/// <summary>
	/// ヒットグリッドを初期化する
	/// </summary>
	void HitGridInit();

	/// <summary>
	/// ヒットグリッドにAABBを追加する
	/// </summary>
	/// <param name="aabb"></param>
	void HitAABB(const AABB_2D& aabb);

	/// <summary>
	/// ヒットグリッドをクリアする
	/// </summary>
	void HitAABBClear() { hitGridPositions_.clear(); }

	/// <summary>
	/// ヒットグリッドの位置を設定する
	/// </summary>
	void SetHitAABBPositions() { hitGrid_->SetPositions(hitGridPositions_); }

private:

	// グリッド
	std::unique_ptr<Line3d> grid_ = nullptr;

	// 判定が取れた所だけ赤くする
	std::unique_ptr<Line3d> hitGrid_ = nullptr;
	std::vector<Vector3> hitGridPositions_;

};

