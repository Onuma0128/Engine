#pragma once

#include <memory>
#include <vector>

#include "Line3d.h"
#include "WorldTransform.h"

#include "animation/joint/Joint.h"

/// <summary>
/// ボーンのデバッグ表示を管理するクラス
/// </summary>
class DebugBone
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="positions"></param>
	void Init(std::vector<Vector3> positions);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画を有効/無効にする
	/// </summary>
	/// <param name="flag"></param>
	void SetEnableDraw(bool flag) { bone_->GetMaterial().enableDraw = flag; }

	/// <summary>
	/// ボーンの座標を設定する
	/// </summary>
	/// <param name="positions"></param>
	void SetPositions(std::vector<Vector3> positions) { bone_->SetPositions(positions); }

	/// <summary>
	/// ボーンの座標を取得する
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="joints"></param>
	/// <returns></returns>
	std::vector<Vector3> GetPositions(
		const WorldTransform& transform,
		const std::vector<Joint>& joints
	);

private:

	std::unique_ptr<Line3d> bone_ = nullptr;

};

