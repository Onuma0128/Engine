#include "DrawGrid.h"

#include <array>

void DrawGrid::Init(const float cell, const float size)
{
#ifdef ENABLE_EDITOR
	// グリッドの初期化
	grid_ = std::make_unique<Line3d>();
	auto gridPos = grid_->CreateGrid(cell, size, 0.1f);
	grid_->Initialize(gridPos);
#endif // ENABLE_EDITOR
}

void DrawGrid::Update()
{
#ifdef ENABLE_EDITOR
	// グリッドの更新
	grid_->Update();
	hitGrid_->Update();
#endif // ENABLE_EDITOR
}

void DrawGrid::SetDrawGrid(bool flag)
{
#ifdef ENABLE_EDITOR
	// グリッドの描画
	grid_->GetMaterial().enableDraw = flag;
	hitGrid_->GetMaterial().enableDraw = flag;
#endif // ENABLE_EDITOR
}

void DrawGrid::SetGridPositions(const float cell, const float size)
{
#ifdef ENABLE_EDITOR
	auto gridPos = grid_->CreateGrid(cell, size, 0.1f);
	grid_->SetPositions(gridPos);
#endif // ENABLE_EDITOR
}

void DrawGrid::HitGridInit()
{
#ifdef ENABLE_EDITOR
	// 当たっているグリッドの初期化
	hitGrid_ = std::make_unique<Line3d>();
	hitGrid_->Initialize(hitGridPositions_);
	hitGrid_->SetColor(Vector3::ExprUnitX);
#endif // ENABLE_EDITOR
}

void DrawGrid::HitAABB(const AABB_2D& aabb)
{
	std::array<Vector3, 4> positions;
	const float kPosY = 0.15f;
	positions[0] = { aabb.min.x,kPosY,aabb.min.y };
	positions[1] = { aabb.max.x,kPosY,aabb.min.y };
	positions[2] = { aabb.max.x,kPosY,aabb.max.y };
	positions[3] = { aabb.min.x,kPosY,aabb.max.y };

	for (uint32_t i = 0; i < positions.size(); ++i) {
		hitGridPositions_.push_back(positions[i]);
		uint32_t index = i + 1;
		if (index >= positions.size()) { index = 0; }
		hitGridPositions_.push_back(positions[index]);
	}
}
