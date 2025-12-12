#pragma once

#include <unordered_set>
#include <string>

namespace CollisionFilter {

	/// <summary>
	/// コライダーの名前をチェックする
	/// </summary>
	/// <param name="name"></相手のColliderNameを入れる>
	/// <returns></returns>
	bool CheckColliderNameEnemy(const std::string& name);
	bool CheckColliderNameFieldObject(const std::string& name);

}