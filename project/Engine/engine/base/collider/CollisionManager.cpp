#include "CollisionManager.h"

#include "Collision3D.h"

void CollisionManager::CheckAllCollisions()
{
    // 今フレームに衝突したペアを格納するコンテナ
    PairSet thisFrame;

    // すべての組み合わせを走査
    std::list<Collider*>::iterator itrA = colliders_.begin();
    for (; itrA != colliders_.end(); ++itrA) {
        Collider* colliderA = *itrA;

        std::list<Collider*>::iterator itrB = itrA;
        itrB++;
        for (; itrB != colliders_.end(); ++itrB) {
            Collider* colliderB = *itrB;

            CheckCollisionPair(colliderA, colliderB, thisFrame);
        }
    }

    // 前フレームにあって今フレームに無いペアを判定
    for (auto& pair : previousFrame_) {
        if (!thisFrame.contains(pair)) {
            pair.first->OnCollisionExit(pair.second);
            pair.second->OnCollisionExit(pair.first);
        }
    }
    previousFrame_.swap(thisFrame);
}

bool CollisionManager::Dispatch(Collider* a, Collider* b)
{
    ColliderType typeA = a->GetMyColliderType();
    ColliderType typeB = b->GetMyColliderType();

    switch (typeA)
    {
    case ColliderType::Sphere:
        switch (typeB) {
        case ColliderType::Sphere:   return Collision3D::SphereSphere(a, b);
        case ColliderType::OBB:      return Collision3D::OBBSphere(b, a);
        case ColliderType::Segment:  return false;
        }
        break;
    case ColliderType::Segment:
        switch (typeB) {
        case ColliderType::Sphere:   return false;
        case ColliderType::OBB:      return Collision3D::OBBSegment(b, a);
        case ColliderType::Segment:  return false;
        }
        break;
    case ColliderType::OBB:
        switch (typeB) {
        case ColliderType::Sphere:   return Collision3D::OBBSphere(a, b);
        case ColliderType::OBB:      return false;
        case ColliderType::Segment:  return Collision3D::OBBSegment(a, b);
        }
        break;
    default:
        break;
    }
}

void CollisionManager::CheckCollisionPair(Collider* a, Collider* b, PairSet& thisFrame)
{
    bool hit = false;

    // 判定の処理
    hit = Dispatch(a, b);
    // 判定が無ければ返す
    if (!hit) return;

    auto key = MakeKey(a, b);
    thisFrame.insert(key);

    if (previousFrame_.contains(key)) {
        // 前フレームも衝突していた
        a->OnCollisionStay(b);
        b->OnCollisionStay(a);
    } else {
        // 今回初めて衝突した
        a->OnCollisionEnter(b);
        b->OnCollisionEnter(a);
    }
}