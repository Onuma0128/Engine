#pragma once
#include <memory>

#include "Line3d.h"

class DrawGrid
{
public:

	void Init();

	void Update();

private:

	// グリッド
	std::unique_ptr<Line3d> grid_ = nullptr;

};

