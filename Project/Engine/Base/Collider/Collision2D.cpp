#include "Collision2D.h"

#include <math.h>
#include <algorithm>

bool NumaEngine::Collision2D::CircleAABB(const NumaEngine::Circle& circle, const NumaEngine::AABB_2D& aabb)
{
    // 円の中心から一番近い点の距離を計算
    NumaEngine::Vector2 closest{
        std::max(aabb.min.x, std::min(circle.center.x, aabb.max.x)),
        std::max(aabb.min.y, std::min(circle.center.y, aabb.max.y))
    };
    NumaEngine::Vector2 diff = circle.center - closest;
    float dist = diff.Length();

    // 距離を比較して判定
    return dist <= (circle.radius);
}

bool NumaEngine::Collision2D::OBBAABB(const NumaEngine::OBB_2D& obb, const NumaEngine::AABB_2D& aabb)
{
    // AABBの中心とサイズを求める
    NumaEngine::Vector2 aabbCenter = (aabb.min + aabb.max) * 0.5f;
    NumaEngine::Vector2 aabbHalfSize = (aabb.max - aabb.min) * 0.5f;

    // OBBの回転行列
    float cosR = std::cosf(obb.rotate);
    float sinR = std::sinf(obb.rotate);

    // AABBの中心をOBB空間に変換
    NumaEngine::Vector2 localCenter = aabbCenter - obb.center;
    NumaEngine::Vector2 rotatedCenter{
        localCenter.x * cosR - localCenter.y * sinR,
        localCenter.x * sinR + localCenter.y * cosR
    };

    // AABBの辺をOBB空間に変換
    NumaEngine::Vector2 rotatedHalfSize = {
        std::abs(aabbHalfSize.x * cosR) + std::abs(aabbHalfSize.y * sinR),
        std::abs(aabbHalfSize.x * sinR) + std::abs(aabbHalfSize.y * cosR)
    };

    // OBBは中心{0,0}のAABB
    NumaEngine::Vector2 obbHalfSize = obb.size;

    // 判定をする
    if (std::abs(rotatedCenter.x) > (obbHalfSize.x + rotatedHalfSize.x)) return false;
    if (std::abs(rotatedCenter.y) > (obbHalfSize.y + rotatedHalfSize.y)) return false;
    return true;

}


