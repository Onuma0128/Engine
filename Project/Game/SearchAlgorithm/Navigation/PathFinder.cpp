#include "PathFinder.h"

#include <queue>
#include <array>
#include <limits>

#include "searchAlgorithm/collision/MapCollision.h"

void PathFinder::Reset()
{
    splineMover_.Reset();
}

void PathFinder::Update(const float speed, float lookAt_t)
{
    splineMover_.Update(speed, lookAt_t);
}

void PathFinder::Search(const Vector3& startW, const Vector3& goalW)
{
    if (mapColl_->GetMapData().empty()) return;

    const uint32_t hSize = static_cast<uint32_t>(mapColl_->GetMapData().size());
    const uint32_t vSize = static_cast<uint32_t>(mapColl_->GetMapData()[0].size());

    auto toIndex = [&](const Vector3& w) -> GridPos {
        // MapCollisionと同じ計算をローカルで行う
        float half = mapColl_->GetHalf();
        float cell = mapColl_->GetCell();
        uint32_t ix = static_cast<uint32_t>((w.x + half) / cell);
        uint32_t iz = static_cast<uint32_t>((w.z + half) / cell);
        ix = std::clamp(ix, 0u, hSize - 1);
        iz = std::clamp(iz, 0u, vSize - 1);
        return { ix, iz };
    };

    // スタートとゴールからグリッドのIndexを割り当てる
    GridPos s = toIndex(startW);
    GridPos g = toIndex(goalW);
    // 推定コストを計算
    auto heur = [&](GridPos a, GridPos b) {
        int dx = int(a.x) - int(b.x);
        int dz = int(a.z) - int(b.z);
        return static_cast<uint32_t>(10 * (std::abs(dx) + std::abs(dz)));
        };
    auto isWalkable = [&](int x, int z)->bool {
        if (x == static_cast<int>(g.x) && z == static_cast<int>(g.z)) return true;
        return mapColl_->GetMapData()[z][x].isEnable;
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
            // スプラインを初期化
            splineMover_.ClearSplinePositions();
            // 経路復元
            Vector3 lastPos{};
            for (A_StarNode* n = cur; n; n = n->parent) {
                splineMover_.PushSplinePositions(mapColl_->GetMapData()[n->pos.z][n->pos.x].center);
                lastPos = mapColl_->GetMapData()[n->pos.z][n->pos.x].center;
            }
            // スタート位置がマップ1ブロック内にあれば削除し追加する
            GridPos last = toIndex(lastPos);
            if (last.x == s.x && last.z == s.z) {
                splineMover_.BackSplinePositions();
            }
            splineMover_.PushSplinePositions(startW);
            // 逆転させる
            splineMover_.ReverseSplinePositions();
            // ゴールの座標を置き換える
            splineMover_.BackSplinePositions();
            splineMover_.PushSplinePositions(goalW);
            // スプライン上の距離を格納する
            splineMover_.ComputeArcLengths();
            return;
        }
        closed.push_back(cur);

        for (size_t i = 0; i < 8; ++i) {
            int nx = int(cur->pos.x) + dx[i];
            int nz = int(cur->pos.z) + dz[i];
            if (nx < 0 || nz < 0 || nx >= int(hSize) || nz >= int(vSize)) continue;
            if (!isWalkable(nx, nz)) continue;

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
    return;   // ゴールに到達できなかった
}
