#include "DrawGrid.h"

#include <array>

void DrawGrid::Init(const float cell, const float size)
{
#ifdef _DEBUG
	// グリッドの初期化
	grid_ = std::make_unique<Line3d>();
	auto gridPos = grid_->CreateGrid(cell, size, 0.1f);
	grid_->Initialize(gridPos);
#endif // _DEBUG
}

void DrawGrid::Update()
{
	// グリッドの更新
	if (grid_ == nullptr) { return; }
	grid_->Update();
	hitGrid_->Update();
}

void DrawGrid::HitGridInit()
{
#ifdef _DEBUG
	// 当たっているグリッドの初期化
	hitGrid_ = std::make_unique<Line3d>();
	hitGrid_->Initialize(hitGridPositions_);
	hitGrid_->SetColor(Vector3{ 1.0f,0.0f,0.0f });
#endif // _DEBUG
}

void DrawGrid::HitAABB(const AABB_2D& aabb)
{
	std::array<Vector3, 4> positions;
	positions[0] = { aabb.min.x,0.11f,aabb.min.y };
	positions[1] = { aabb.max.x,0.11f,aabb.min.y };
	positions[2] = { aabb.max.x,0.11f,aabb.max.y };
	positions[3] = { aabb.min.x,0.11f,aabb.max.y };

	for (uint32_t i = 0; i < positions.size(); ++i) {
		hitGridPositions_.push_back(positions[i]);
		uint32_t index = i + 1;
		if (index >= positions.size()) { index = 0; }
		hitGridPositions_.push_back(positions[index]);
	}
}
