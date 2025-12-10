#pragma once
#include <list>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

#include "Collider.h"

class CollisionManager
{
private:

    // シングルトンインスタンス
    static std::unique_ptr<CollisionManager> instance_;

public:

    CollisionManager() = default;
    ~CollisionManager() = default;
    CollisionManager(CollisionManager&) = delete;
    CollisionManager& operator=(CollisionManager&) = delete;

    // シングルトンインスタンスの取得
    static CollisionManager* GetInstance();

    /// <summary>
    /// すべての衝突判定を行う
    /// </summary>
    void CheckAllCollisions();

    /// <summary>
    /// コライダーを追加する
    /// </summary>
    /// <param name="c"></追加したいコライダー>
    void AddCollider(Collider* c) { colliders_.push_back(c); }

    /// <summary>
    /// コライダーを削除する
    /// </summary>
    /// <param name="c"></削除したいコライダー>
    void RemoveCollider(Collider* c) {
        colliders_.remove(c);
        for (auto it = previousFrame_.begin(); it != previousFrame_.end(); )
        {
            if (it->first == c || it->second == c) {
                it = previousFrame_.erase(it);
            } else {
                ++it;
            }
        }
    }

    /// <summary>
    /// デバッグ用のImGuiを表示する
    /// </summary>
    void DebugImGui();

    /// <summary>
    /// コライダーを全てクリアする
    /// </summary>
    void ClearCollider() { colliders_.clear(); }

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() {
        colliders_.clear();
        previousFrame_.clear();
		instance_ = nullptr;
	}

private:
    
    /// <summary>
    /// コライダーのペアを作成する
    /// </summary>
    static std::pair<Collider*, Collider*> MakeKey(Collider* a, Collider* b) {
        return (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
    }

    /// <summary>
    /// コライダーのペアをハッシュ化する
    /// </summary>
    struct PairHasher {
        size_t operator()(const std::pair<Collider*, Collider*>& p) const noexcept {
            return std::hash<Collider*>()(p.first) ^ (std::hash<Collider*>()(p.second) << 1);
        }
    };
    using PairSet = std::unordered_set<std::pair<Collider*, Collider*>, PairHasher>;

    /// <summary>
    /// コライダーのペアをディスパッチする
    /// </summary>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <returns></returns>
    bool Dispatch(Collider* a, Collider* b);

    /// <summary>
    /// コライダーのペアをチェックする
    /// </summary>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <param name="thisFrame"></param>
    void CheckCollisionPair(Collider* a, Collider* b, PairSet& thisFrame);

    /// <summary>
    /// セルのキー
    /// </summary>
    struct CellKey { 
        int x, y, z;

        bool operator==(const CellKey& other) const noexcept {
            return x == other.x && y == other.y && z == other.z;
		}
    };

    /// <summary>
    /// セルのキーをハッシュ化する
    /// </summary>
    struct CellKeyHash {
        size_t operator()(const CellKey& key) const noexcept {
            size_t h = 1469598103934665603ull;
            auto minKey = [&](int v) {h ^= std::hash<int>()(v); h *= 1099511628211ull; };
            minKey(key.x); minKey(key.y); minKey(key.z);
            return h;
        }
    };

    /// <summary>
    /// ワールド座標をセルのキーに変換する
    /// </summary>
    CellKey ToCell(const Vector3& position, float cellSize) const noexcept {
        return CellKey{
            static_cast<int>(std::floor(position.x / cellSize)),
            static_cast<int>(std::floor(position.y / cellSize)),
            static_cast<int>(std::floor(position.z / cellSize))
        };
	}

private:

    std::list<Collider*> colliders_;
    PairSet previousFrame_;
    bool debugColliderEnable_ = false;
};