#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

/// <summary>
/// 2D変換を表す構造体
/// </summary>
struct Transform2D {
	Vector2 size;
	float rotate;
	Vector2 position;
};

/// <summary>
/// オイラー角による変換を表す構造体
/// </summary>
struct EulerTransform {
	Vector3 scale;
	Vector3 rotation;
	Vector3 translation;
};

/// <summary>
/// 3D変換を表す構造体
/// </summary>
struct Transform3D {
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
};