#pragma once
#include <memory>
#include <array>

#include "Object3d.h"

class Player;

/// <summary>
/// 仲間の予測オブジェクトクラス
/// </summary>
class PredictionObjects
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="offset"></param>
	void Update(const Vector3& start, const Vector3& goal);

private:

	static const size_t objSize_ = 3;
	std::array<std::unique_ptr<Object3d>, objSize_> predictionObjects_;
	
};
