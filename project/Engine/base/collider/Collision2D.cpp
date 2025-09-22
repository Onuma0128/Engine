#include "Collision2D.h"

#include <math.h>
#include <algorithm>

bool Collision2D::CircleAABB(const Circle& circle, const AABB_2D& aabb)
{
    // 円の中心から一番近い点の距離を計算
    Vector2 closest{
        std::clamp(circle.center.x,aabb.min.x,aabb.max.x),
        std::clamp(circle.center.y,aabb.min.y,aabb.max.y)
    };
    Vector2 diff = circle.center - closest;
    float dist = diff.Length();

    // 距離を比較して判定
    return dist <= (circle.radius);
}

bool Collision2D::OBBAABB(const OBB_2D& obb, const AABB_2D& aabb)
{
    // AABBの中心とサイズを求める
    Vector2 aabbCenter = (aabb.min + aabb.max) * 0.5f;
    Vector2 aabbHalfSize = (aabb.max - aabb.min) * 0.5f;

    // OBBの回転行列
    float cosR = std::cosf(obb.rotate);
    float sinR = std::sinf(obb.rotate);

    // AABBの中心をOBB空間に変換
    Vector2 localCenter = aabbCenter - obb.center;
    Vector2 rotatedCenter{
        localCenter.x * cosR - localCenter.y * sinR,
        localCenter.x * sinR + localCenter.y * cosR
    };

    // AABBの辺をOBB空間に変換
    Vector2 rotatedHalfSize = {
        std::abs(aabbHalfSize.x * cosR) + std::abs(aabbHalfSize.y * sinR),
        std::abs(aabbHalfSize.x * sinR) + std::abs(aabbHalfSize.y * cosR)
    };

    // OBBは中心{0,0}のAABB
    Vector2 obbHalfSize = obb.size;

    // 判定をする
    if (std::abs(rotatedCenter.x) > (obbHalfSize.x + rotatedHalfSize.x)) return false;
    if (std::abs(rotatedCenter.y) > (obbHalfSize.y + rotatedHalfSize.y)) return false;
    return true;

}
