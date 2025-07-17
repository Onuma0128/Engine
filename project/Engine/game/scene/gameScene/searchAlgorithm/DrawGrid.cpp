#include "DrawGrid.h"

void DrawGrid::Init()
{
	// グリッドの初期化
	grid_ = std::make_unique<Line3d>();
	auto gridPos = grid_->CreateGrid(2.0f, 120.0f, 0.1f);
	grid_->Initialize(gridPos);
}

void DrawGrid::Update()
{
	// グリッドの更新
	grid_->Update();
}