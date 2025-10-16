#pragma once

#include <memory>
#include <vector>

#include "Line3d.h"
#include "WorldTransform.h"

#include "animation/joint/Joint.h"

class DebugBone
{
public:

	void Init(std::vector<Vector3> positions);

	void Update();

	void SetEnableDraw(bool flag) { bone_->GetMaterial().enableDraw = flag; }
	void SetPositions(std::vector<Vector3> positions) { bone_->SetPositions(positions); }

	// debugBoneの頂点座標のゲッター
	std::vector<Vector3> GetPositions(
		const WorldTransform& transform,
		const std::vector<Joint>& joints
	);

private:

	std::unique_ptr<Line3d> bone_ = nullptr;

};

