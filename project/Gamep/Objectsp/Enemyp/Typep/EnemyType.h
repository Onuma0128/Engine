#pragma once

/// <summary>
/// 敵の種類を表す列挙体
/// </summary>
enum class EnemyType 
{
    kMelee,          // 近接
    kRanged,         // 遠距離
    kShieldBearer,   // 盾持ち
    kRangedElite,    // 遠距離（強化版）
};
