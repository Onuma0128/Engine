#pragma once

#include <vector>

#include "Collider.h"
#include "Vector3.h"

namespace {
    struct OctAABB {
        Vector3 min, max;
        bool Intersects(const OctAABB& o) const noexcept {
            return !(max.x < o.min.x || min.x > o.max.x ||
                max.y < o.min.y || min.y > o.max.y ||
                max.z < o.min.z || min.z > o.max.z);
        }
        bool Contains(const OctAABB& o) const noexcept {
            return (min.x <= o.min.x && max.x >= o.max.x &&
                min.y <= o.min.y && max.y >= o.max.y &&
                min.z <= o.min.z && max.z >= o.max.z);
        }
        Vector3 Center() const noexcept { return (min + max) * 0.5f; }
        Vector3 Size() const noexcept { return (max - min); }
    };

    // コライダーとそのAABBを束ねる
    struct OctItem {
        Collider* col;
        OctAABB   box;
    };

    struct OctNode {
        OctAABB bounds;
        int depth = 0;
        int maxDepth = 8;
        int capacity = 8;
        std::vector<OctItem> items;
        std::unique_ptr<OctNode> children[8]; // 8分木

        bool IsLeaf() const {
            for (auto& c : children) if (c) return false;
            return true;
        }

        void Subdivide() {
            Vector3 c = bounds.Center();
            Vector3 s = bounds.Size() * 0.5f;
            // 8子ノードを作成（min/max を組み合わせ）
            for (int i = 0; i < 8; ++i) {
                Vector3 offset{
                    (i & 1) ? s.x * 0.5f : -s.x * 0.5f,
                    (i & 2) ? s.y * 0.5f : -s.y * 0.5f,
                    (i & 4) ? s.z * 0.5f : -s.z * 0.5f
                };
                OctAABB childBox;
                childBox.min = { std::min(c.x, c.x + offset.x) - s.x * 0.5f,
                                 std::min(c.y, c.y + offset.y) - s.y * 0.5f,
                                 std::min(c.z, c.z + offset.z) - s.z * 0.5f };
                childBox.max = { std::max(c.x, c.x + offset.x) + s.x * 0.5f,
                                 std::max(c.y, c.y + offset.y) + s.y * 0.5f,
                                 std::max(c.z, c.z + offset.z) + s.z * 0.5f };
                children[i] = std::make_unique<OctNode>();
                children[i]->bounds = childBox;
                children[i]->depth = depth + 1;
                children[i]->maxDepth = maxDepth;
                children[i]->capacity = capacity;
            }
        }

        // 完全に収まる子があるなら子へ。それ以外（跨る）はこのノードに保持
        void Insert(const OctItem& it) {
            if (!IsLeaf()) {
                for (auto& ch : children) {
                    if (ch->bounds.Contains(it.box)) {
                        ch->Insert(it);
                        return;
                    }
                }
            }
            items.push_back(it);

            if (IsLeaf() && depth < maxDepth && (int)items.size() > capacity) {
                Subdivide();
                // 再配分
                std::vector<OctItem> keep;
                keep.reserve(items.size());
                for (auto& e : items) {
                    bool pushed = false;
                    for (auto& ch : children) {
                        if (ch->bounds.Contains(e.box)) {
                            ch->Insert(e);
                            pushed = true;
                            break;
                        }
                    }
                    if (!pushed) keep.push_back(e);
                }
                items.swap(keep);
            }
        }

        // 子孫から全アイテム収集（比較用）
        void CollectAll(std::vector<OctItem>& out) const {
            out.insert(out.end(), items.begin(), items.end());
            if (!IsLeaf()) {
                for (auto& ch : children) ch->CollectAll(out);
            }
        }

        // このノード内の候補ペアを out へ格納
        void GatherPairs(std::vector<std::pair<Collider*, Collider*>>& out) const {
            // 1) このノード保持分の総当たり
            for (size_t i = 0; i < items.size(); ++i) {
                for (size_t j = i + 1; j < items.size(); ++j) {
                    if (items[i].box.Intersects(items[j].box)) {
                        out.emplace_back(items[i].col, items[j].col);
                    }
                }
            }
            // 2) 親に残ったもの vs 各子サブツリー
            if (!IsLeaf()) {
                for (auto& ch : children) {
                    // 親保持分と子すべて
                    std::vector<OctItem> subtree;
                    ch->CollectAll(subtree);
                    for (auto& a : items) {
                        for (auto& b : subtree) {
                            if (a.box.Intersects(b.box)) {
                                out.emplace_back(a.col, b.col);
                            }
                        }
                    }
                    // 子の中も再帰
                    ch->GatherPairs(out);
                }
            }
        }
    };

    // 全コライダーから包囲AABBを作る（八分木のルート）
    inline OctAABB ComputeSceneBounds(const std::vector<OctItem>& items) {
        OctAABB b{};
        if (items.empty()) {
            b.min = { -100.f, -100.f, -100.f };
            b.max = { 100.f,  100.f,  100.f };
            return b;
        }
        b.min = items[0].box.min;
        b.max = items[0].box.max;
        for (auto& it : items) {
            b.min.x = std::min(b.min.x, it.box.min.x);
            b.min.y = std::min(b.min.y, it.box.min.y);
            b.min.z = std::min(b.min.z, it.box.min.z);
            b.max.x = std::max(b.max.x, it.box.max.x);
            b.max.y = std::max(b.max.y, it.box.max.y);
            b.max.z = std::max(b.max.z, it.box.max.z);
        }
        // 立方体化（バランス良く分割するため）
        Vector3 c = b.Center();
        Vector3 s = b.Size();
        float m = std::max({ s.x, s.y, s.z }) * 0.5f;
        b.min = { c.x - m, c.y - m, c.z - m };
        b.max = { c.x + m, c.y + m, c.z + m };
        return b;
    }
}
