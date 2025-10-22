#pragma once

/// <summary>
/// 敵の種類を表す列挙体
/// </summary>
enum class EnemyType 
{
    Melee,          // 近接
    Ranged,         // 遠距離
    ShieldBearer,   // 盾持ち
    RangedElite,    // 遠距離（強化版）
};
