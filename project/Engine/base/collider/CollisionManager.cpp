#include "CollisionManager.h"

#include "Collision3D.h"

#include "imgui.h"

void CollisionManager::CheckAllCollisions()
{
    // デバッグ用
    DebugImGui();

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
    std::vector<std::pair<Collider*, Collider*>> exited;
    for (auto& pair : previousFrame_) {
        if (!thisFrame.contains(pair)) { exited.push_back(pair); }
    }

    for (auto& pair : exited) {
        if (pair.first && pair.second) {
            pair.first->OnCollisionExit(pair.second);
            pair.first->SetColor(Vector3{ 1.0f,1.0f,1.0f });
            pair.second->OnCollisionExit(pair.first);
            pair.second->SetColor(Vector3{ 1.0f,1.0f,1.0f });
        }
    }

    previousFrame_.swap(thisFrame);
}

void CollisionManager::DebugImGui()
{
#ifdef _DEBUG

    bool change = debugColliderEnable_;
    ImGui::Begin("ColliderManager");
    ImGui::Checkbox("debugCollider", &debugColliderEnable_);
    ImGui::End();

    if (debugColliderEnable_ != change) {
        for (auto& collider : colliders_) {
            collider->SetDebugDrawEnable(debugColliderEnable_);
        }
    }

#endif // _DEBUG
}

bool CollisionManager::Dispatch(Collider* a, Collider* b)
{
    // お互いのTypeを取得
    ColliderType typeA = a->GetMyColliderType();
    ColliderType typeB = b->GetMyColliderType();

    // お互いのActiveがONなら判定を取る、どちらかがOFFなら判定を取らない
    if (!a->GetActive() || !b->GetActive()) {
        return false;
    }
    // お互いのTargetに入っているかを判定する
    if (a->GetTargetColliderName().empty() || b->GetTargetColliderName().empty()) {
        return false;
    } else {
        const auto& nameA = a->GetColliderName();
        const auto& nameB = b->GetColliderName();
        bool hit = false;
        for (const auto& targetA : a->GetTargetColliderName()) {
            if (nameB == targetA) {
                for (const auto& targetB : b->GetTargetColliderName()) {
                    if (nameA == targetB) {
                        hit = true;
                        break;
                    }
                }
                if (hit) { break; }
            }
        }
        if (!hit) { return false; }
    }

    // お互いのTypeで判定を取る
    switch (typeA)
    {
    case ColliderType::Sphere:
        switch (typeB) {
        case ColliderType::Sphere:   return Collision3D::SphereSphere(a, b);
        case ColliderType::OBB:      return Collision3D::OBBSphere(b, a);
        case ColliderType::Segment:  return Collision3D::SphereSegment(a, b);
        }
        break;
    case ColliderType::Segment:
        switch (typeB) {
        case ColliderType::Sphere:   return Collision3D::SphereSegment(b,a);
        case ColliderType::OBB:      return Collision3D::OBBSegment(b, a);
        case ColliderType::Segment:  return false;
        }
        break;
    case ColliderType::OBB:
        switch (typeB) {
        case ColliderType::Sphere:   return Collision3D::OBBSphere(a, b);
        case ColliderType::OBB:      return Collision3D::OBBOBB(a, b);
        case ColliderType::Segment:  return Collision3D::OBBSegment(a, b);
        }
        break;
    default:
        break;
    }

    return false;
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
        a->SetColor(Vector3{ 1.0f,0.0f,0.0f });
        b->OnCollisionEnter(a);
        b->SetColor(Vector3{ 1.0f,0.0f,0.0f });
    }
}