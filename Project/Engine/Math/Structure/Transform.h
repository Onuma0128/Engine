#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

/// <summary>
/// 2D変換を表す構造体
/// </summary>
struct Transform2D {
	NumaEngine::Vector2 size;
	float rotate;
	NumaEngine::Vector2 position;
};

/// <summary>
/// オイラー角による変換を表す構造体
/// </summary>
struct EulerTransform {
	NumaEngine::Vector3 scale;
	NumaEngine::Vector3 rotation;
	NumaEngine::Vector3 translation;
};

/// <summary>
/// 3D変換を表す構造体
/// </summary>
struct Transform3D {
	NumaEngine::Vector3 scale;
	NumaEngine::Quaternion rotation;
	NumaEngine::Vector3 translation;
};



