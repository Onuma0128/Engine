#include "MapCollision.h"

#include "imgui.h"

#include "Collider.h"

void MapCollision::Init(SceneJsonLoader loader)
{
	// フィールドに配置されているObjectを取得する
	for (auto it = loader.GetData().begin(); it != loader.GetData().end();) {
		if (it->second.groupName == "FieldObject") {
			auto& object = it->second;
			if (object.collider.active) {
				if (object.collider.type == ColliderType::OBB) {
					OBB_2D obb = {
						.center = object.collider.defPosition,
						.rotate = object.collider.rotate,
						.size = {object.collider.size.x,object.collider.size.z}
					};
					objects_obb_.push_back(obb);
				} else {
					Circle circle = {
						.center = object.collider.defPosition,
						.radius = object.collider.radius
					};
					objects_circle_.push_back(circle);
				}
			}
		}
		++it;
	}

	cell_ = 1.0f;			// グリッドの1マスの幅
	size_ = 120.0f;			// グリッドの全体サイズ
	half_ = size_ * 0.5f;	// グリッドの半径

	// グリッドを初期化
	grid_ = std::make_unique<DrawGrid>();
	grid_->Init(cell_, size_);

	// マップを作成する
	CreateMap();

	// フィールドオブジェクトとの判定
	CreateMapCollision();

	grid_->HitGridInit();
}

void MapCollision::Update()
{
#ifdef _DEBUG

	ImGui::Begin("map");
	ImGui::DragFloat("size", &size_, 0.01f, 0.01f, 1000.0f);
	ImGui::DragFloat("cell", &cell_, 0.01f, 0.1f, 10.0f);
	if (ImGui::Button("Reload")) {
		// マップを再ロードする
		ReloadMap();
	}
	ImGui::SameLine();
	ImGui::Checkbox("debugLine", &debugLine_);
	ImGui::End();

#endif // _DEBUG

	grid_->SetDrawGrid(debugLine_);
	grid_->Update();
}

void MapCollision::CreateMap()
{
	// サイズを決める
	uint32_t maxSize = static_cast<uint32_t>(size_ / cell_);
	mapDatas_.resize(static_cast<size_t>(maxSize));
	for (uint32_t i = 0; i < maxSize; ++i) {			// z軸(縦)
		mapDatas_[i].resize(maxSize);
		for (uint32_t j = 0; j < maxSize; ++j) {		// x軸(横)
			Maptip map;
			float minX = -half_ + j * cell_;
			float minY = -half_ + i * cell_;
			map.aabb.min = { minX, minY };
			map.aabb.max = { minX + cell_, minY + cell_ };
			Vector2 center = (map.aabb.min + map.aabb.max) * 0.5f;
			map.center = { center.x,0.0f,center.y };
			map.isEnable = true;
			mapDatas_[i][j] = map;
		}
	}
}

void MapCollision::CreateMapCollision()
{
	// フィールドオブジェクトとの判定
	for (auto& maptips : mapDatas_) {
		for (auto& block : maptips) {
			// OBB
			for (auto& obb : objects_obb_) {
				if (Collision2D::OBBAABB(obb, block.aabb)) {
					block.isEnable = false;
					grid_->HitAABB(block.aabb);
					break;
				}
			}
			if (!block.isEnable) { continue; }
			// Circle
			for (auto& circle : objects_circle_) {
				if (Collision2D::CircleAABB(circle, block.aabb)) {
					block.isEnable = false;
					grid_->HitAABB(block.aabb);
					break;
				}
			}
		}
	}
}

void MapCollision::ReloadMap()
{
	// グリッドの半径を作成
	half_ = size_ * 0.5f;
	// グリッドを作成
	grid_->SetGridPositions(cell_, size_);
	grid_->HitAABBClear();
	// マップを作成する
	CreateMap();
	// フィールドオブジェクトとの判定
	CreateMapCollision();
	grid_->SetHitAABBPositions();
}
