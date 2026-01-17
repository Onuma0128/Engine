#include "CollisionFilter.h"

bool CollisionFilter::CheckColliderNameEnemy(const std::string& name)
{
	static const std::unordered_set<std::string> names =
	{ "EnemyMelee","EnemyRanged","EnemyShieldBearer","EnemyRangedElite","BossAttack" };
	return names.contains(name);
}

bool CollisionFilter::CheckColliderNameFieldObject(const std::string& name)
{
	static const std::unordered_set<std::string> names =
	{ "Building","DeadTree","fence","Bush","ShortStoneWall","StoneWall" };
	return names.contains(name);
}

bool CollisionFilter::CheckColliderNameCompanion(const std::string& name)
{
	static const std::unordered_set<std::string> names =
	{ "MuscleCompanion","FollowerMuscleCompanion","BlowDashMuscleCompanion",
		"MuscleCompanionAttack","SearchDashMuscleCompanion" };
	return names.contains(name);
}
