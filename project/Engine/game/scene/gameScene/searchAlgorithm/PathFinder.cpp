#include "PathFinder.h"

#include <queue>
#include <array>
#include <limits>

bool PathFinder::Search(const Vector3& startW, const Vector3& goalW, std::vector<Vector3>& outputPositions)
{
    if (mapDatas_.empty()) return false;

    const uint32_t hSize = static_cast<uint32_t>(mapDatas_.size());
    const uint32_t vSize = static_cast<uint32_t>(mapDatas_[0].size());

    auto toIndex = [&](const Vector3& w) -> GridPos {
        // MapCollisionと同じ計算をローカルで行う
        float half = static_cast<float>(hSize) * 0.5f;
        uint32_t ix = static_cast<uint32_t>((half + w.x));
        uint32_t iz = static_cast<uint32_t>((half + w.z));
        return { std::clamp(ix, 0u, hSize - 1), std::clamp(iz, 0u, vSize - 1) };
    };

    // スタートとゴールからグリッドのIndexを割り当てる
    GridPos s = toIndex(startW);
    GridPos g = toIndex(goalW);
    if (!mapDatas_[s.z][s.x].isEnable || !mapDatas_[g.z][g.x].isEnable) return false;
    // 推定コストを計算
    auto heur = [&](GridPos a, GridPos b) {
        int dx = int(a.x) - int(b.x);
        int dz = int(a.z) - int(b.z);
        return static_cast<uint32_t>(10 * (std::abs(dx) + std::abs(dz)));
        };
    auto cmp = [](A_StarNode* a, A_StarNode* b) { return a->score > b->score; };
    std::priority_queue<A_StarNode*, std::vector<A_StarNode*>, decltype(cmp)> open(cmp);

    std::vector<std::vector<A_StarNode>> nodes(vSize,
        std::vector<A_StarNode>(hSize));

    // 初期化
    for (uint32_t z = 0; z < vSize; ++z)
        for (uint32_t x = 0; x < hSize; ++x) {
            nodes[z][x] = { {x,z}, std::numeric_limits<uint32_t>::max(),
                             0, std::numeric_limits<uint32_t>::max(), nullptr };
        }

    A_StarNode* start = &nodes[s.z][s.x];
    start->actualCost = 0;
    start->estimatedCost = heur(s, g);
    start->score = start->estimatedCost;
    open.push(start);

    constexpr std::array<int, 8> dx{ 1,-1,0,0, 1,1,-1,-1 };
    constexpr std::array<int, 8> dz{ 0,0,1,-1, 1,-1,1,-1 };
    constexpr std::array<uint32_t, 8> cost{ 10,10,10,10, 14,14,14,14 };

    std::vector<A_StarNode*> closed;
    while (!open.empty()) {
        A_StarNode* cur = open.top(); open.pop();
        if (cur->pos.x == g.x && cur->pos.z == g.z) {     // ゴール
            // 経路復元
            for (A_StarNode* n = cur; n; n = n->parent) {
                outputPositions.push_back(mapDatas_[n->pos.z][n->pos.x].center);
            }
            std::reverse(outputPositions.begin(), outputPositions.end());
            return true;
        }
        closed.push_back(cur);

        for (size_t i = 0; i < 8; ++i) {
            int nx = int(cur->pos.x) + dx[i];
            int nz = int(cur->pos.z) + dz[i];
            if (nx < 0 || nz < 0 || nx >= int(hSize) || nz >= int(vSize)) continue;
            if (!mapDatas_[nz][nx].isEnable) continue;

            A_StarNode* nei = &nodes[nz][nx];
            uint32_t gNew = cur->actualCost + cost[i];
            if (gNew < nei->actualCost) {
                nei->actualCost = gNew;
                nei->estimatedCost = heur(nei->pos, g);
                nei->score = nei->actualCost + nei->estimatedCost;
                nei->parent = cur;
                open.push(nei);
            }
        }
    }
    return false;   // ゴールに到達できなかった
}
