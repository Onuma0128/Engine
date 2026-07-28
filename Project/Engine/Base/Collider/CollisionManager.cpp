#include "CollisionManager.h"

#include "Collision3D.h"

#include "imgui.h"

std::unique_ptr<NumaEngine::CollisionManager> NumaEngine::CollisionManager::instance_ = nullptr;

NumaEngine::CollisionManager* NumaEngine::CollisionManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = std::make_unique<NumaEngine::CollisionManager>();
    }
    return instance_.get();
}

void NumaEngine::CollisionManager::CheckAllCollisions()
{
    // デバッグ用
    DebugImGui();

    // 今フレームに衝突したペアを格納するコンテナ
    NumaEngine::CollisionManager::PairSet thisFrame;

    // すべての組み合わせを走査
    std::list<NumaEngine::Collider*>::iterator itrA = colliders_.begin();
    for (; itrA != colliders_.end(); ++itrA) {
        NumaEngine::Collider* colliderA = *itrA;

        std::list<NumaEngine::Collider*>::iterator itrB = itrA;
        itrB++;
        for (; itrB != colliders_.end(); ++itrB) {
            NumaEngine::Collider* colliderB = *itrB;

            CheckCollisionPair(colliderA, colliderB, thisFrame);
        }
    }

    // 前フレームにあって今フレームに無いペアを判定
    std::vector<std::pair<NumaEngine::Collider*, NumaEngine::Collider*>> exited;
    for (auto& pair : previousFrame_) {
        if (!thisFrame.contains(pair)) { exited.push_back(pair); }
    }

    for (auto& pair : exited) {
        if (pair.first && pair.second) {
            pair.first->OnCollisionExit(pair.second);
            pair.first->SetColor(NumaEngine::Vector3{ 1.0f,1.0f,1.0f });
            pair.second->OnCollisionExit(pair.first);
            pair.second->SetColor(NumaEngine::Vector3{ 1.0f,1.0f,1.0f });
        }
    }

    previousFrame_.swap(thisFrame);
}

void NumaEngine::CollisionManager::DebugImGui()
{
#ifdef ENABLE_EDITOR

    bool change = debugColliderEnable_;
    ImGui::Begin("ColliderManager");
    ImGui::Checkbox("debugCollider", &debugColliderEnable_);
    ImGui::End();

    if (debugColliderEnable_ != change) {
        for (auto& collider : colliders_) {
            collider->SetDebugDrawEnable(debugColliderEnable_);
        }
    }

#endif // ENABLE_EDITOR
}

bool NumaEngine::CollisionManager::Dispatch(NumaEngine::Collider* a, NumaEngine::Collider* b)
{
    // お互いのTypeを取得
    NumaEngine::ColliderType typeA = a->GetMyColliderType();
    NumaEngine::ColliderType typeB = b->GetMyColliderType();

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
    case NumaEngine::ColliderType::kSphere:
        switch (typeB) {
        case NumaEngine::ColliderType::kSphere:   return NumaEngine::Collision3D::SphereSphere(a, b);
        case NumaEngine::ColliderType::kOBB:      return NumaEngine::Collision3D::OBBSphere(b, a);
        case NumaEngine::ColliderType::kSegment:  return NumaEngine::Collision3D::SphereSegment(a, b);
        }
        break;
    case NumaEngine::ColliderType::kSegment:
        switch (typeB) {
        case NumaEngine::ColliderType::kSphere:   return NumaEngine::Collision3D::SphereSegment(b,a);
        case NumaEngine::ColliderType::kOBB:      return NumaEngine::Collision3D::OBBSegment(b, a);
        case NumaEngine::ColliderType::kSegment:  return false;
        }
        break;
    case NumaEngine::ColliderType::kOBB:
        switch (typeB) {
        case NumaEngine::ColliderType::kSphere:   return NumaEngine::Collision3D::OBBSphere(a, b);
        case NumaEngine::ColliderType::kOBB:      return NumaEngine::Collision3D::OBBOBB(a, b);
        case NumaEngine::ColliderType::kSegment:  return NumaEngine::Collision3D::OBBSegment(a, b);
        }
        break;
    default:
        break;
    }

    return false;
}

void NumaEngine::CollisionManager::CheckCollisionPair(NumaEngine::Collider* a, NumaEngine::Collider* b, NumaEngine::CollisionManager::PairSet& thisFrame)
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
        a->SetColor(NumaEngine::Vector3{ 1.0f,0.0f,0.0f });
        b->OnCollisionEnter(a);
        b->SetColor(NumaEngine::Vector3{ 1.0f,0.0f,0.0f });
    }
}
