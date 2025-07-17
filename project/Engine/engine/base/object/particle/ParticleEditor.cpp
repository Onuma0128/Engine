#include "ParticleEditor.h"

#include "imgui.h"

#include "TextureManager.h"

void ParticleEditor::Initialize(std::string filePath)
{
	parameters_.Init(filePath);

	if (!parameters_.Load()) {

		/* ==================== Particle全体の固定値 ==================== */

		// Texture
		parameters_.Set("Texture", "white1x1.png");
		baseEmitter_.texture = "white1x1.png";
		// 選択できるテクスチャ
		auto items = TextureManager::GetInstance()->GetTextures();
		// テクスチャを検索
		for (int i = 0; i < items.size(); ++i) {
			if (baseEmitter_.texture == items[i]) { textureIndex_ = i; }
		}
		// Model
		parameters_.Set("Model", 0);
		// BlendMode
		parameters_.Set("BlendMode", 0);
		// 発生させるか
		parameters_.Set("Move", false);
		// 発生数
		parameters_.Set("EmitCount", 0);
		// 発生までの時間
		parameters_.Set("FrequencyTime", 0.1f);
		// 発生してから消えるまでの時間
		parameters_.Set("LifeTime", 0.5f);
		// 加速度
		parameters_.Set("Acceleration", Vector3{});
		// 加速度が掛かるフィールドのスケール
		parameters_.Set("AcceleField", false);
		parameters_.Set("MinAcceleField", Vector3{});
		parameters_.Set("MaxAcceleField", Vector3{});
		// パーティクルが反射するY軸
		parameters_.Set("Reflect", false);
		parameters_.Set("ReflectY", 0.0f);

		/* ==================== Particle1つのランダム値 ==================== */

		// カラー
		parameters_.Set("Color", Vector3{});
		parameters_.Set("uvTranslation", Vector3{});
		// スケール
		parameters_.Set("MinScale", Vector3{});
		parameters_.Set("MaxScale", Vector3{});
		parameters_.Set("ChangeScale", false);
		parameters_.Set("EndScale", Vector3{});
		// 速度
		parameters_.Set("MinVelocity", Vector3{});
		parameters_.Set("MaxVelocity", Vector3{});
		// 回転
		parameters_.Set("Billboard", true);
		parameters_.Set("MinRotate", Vector3{});
		parameters_.Set("MaxRotate", Vector3{});
		parameters_.Set("MinRotateSpeed", Vector3{});
		parameters_.Set("MaxRotateSpeed", Vector3{});
		
		/* ==================== Emitterの固定値 ==================== */

		// サイズ
		parameters_.Set("MinSize", Vector3{});
		parameters_.Set("MaxSize", Vector3{});
		// Transform
		parameters_.Set("Position", Vector3{});
		// Rotate,Positionを固定するか
		parameters_.Set("Look", false);

	} else {

		/* ==================== Particle全体の固定値 ==================== */

		// Texture
		baseEmitter_.texture = parameters_.Get<std::string>("Texture", "white1x1.png");
		// 選択できるテクスチャ
		auto items = TextureManager::GetInstance()->GetTextures();
		// テクスチャを検索
		for (int i = 0; i < items.size(); ++i) {
			if (baseEmitter_.texture == items[i]) { textureIndex_ = i; }
		}
		// Model
		baseEmitter_.model_ = parameters_.Get<int>("Model", static_cast<int>(baseEmitter_.model_));
		modelIndex_ = baseEmitter_.model_;
		// BlendMode
		baseEmitter_.blendMode_ = parameters_.Get<int>("BlendMode", static_cast<int>(baseEmitter_.blendMode_));
		blendIndex_ = baseEmitter_.blendMode_;
		// 発生数
		baseEmitter_.count = parameters_.Get<int>("EmitCount", static_cast<int>(baseEmitter_.count));
		// 発生までの時間
		baseEmitter_.isMoveStart = parameters_.Get<bool>("Move", baseEmitter_.isMoveStart);
		baseEmitter_.frequency = parameters_.Get<float>("FrequencyTime", baseEmitter_.frequency);
		// 発生してから消えるまでの時間
		baseEmitter_.lifeTime = parameters_.Get<float>("LifeTime", baseEmitter_.lifeTime);
		// 加速度
		baseEmitter_.acceleration = parameters_.Get<Vector3>("Acceleration", baseEmitter_.acceleration);
		// 加速度が掛かるフィールドのスケール
		baseEmitter_.isFieldStart = parameters_.Get<bool>("AcceleField", baseEmitter_.isFieldStart);
		baseEmitter_.minAccelerField = parameters_.Get<Vector3>("MinAcceleField", baseEmitter_.minAccelerField);
		baseEmitter_.maxAccelerField = parameters_.Get<Vector3>("MaxAcceleField", baseEmitter_.maxAccelerField);
		// パーティクルが反射するY軸
		baseEmitter_.isReflect = parameters_.Get<bool>("Reflect", baseEmitter_.isReflect);
		baseEmitter_.reflectY = parameters_.Get<float>("ReflectY", baseEmitter_.reflectY);

		/* ==================== Particle1つのランダム値 ==================== */

		// カラー
		baseEmitter_.color = parameters_.Get<Vector3>("Color", baseEmitter_.color);
		baseEmitter_.uvTranslation = parameters_.Get<Vector3>("uvTranslation", baseEmitter_.uvTranslation);
		// スケール
		baseEmitter_.minScale = parameters_.Get<Vector3>("MinScale", baseEmitter_.minScale);
		baseEmitter_.maxScale = parameters_.Get<Vector3>("MaxScale", baseEmitter_.maxScale);
		baseEmitter_.isChangeScale = parameters_.Get<bool>("ChangeScale", baseEmitter_.isChangeScale);
		baseEmitter_.endScale = parameters_.Get<Vector3>("EndScale", baseEmitter_.endScale);
		// 速度
		baseEmitter_.minVelocity = parameters_.Get<Vector3>("MinVelocity", baseEmitter_.minVelocity);
		baseEmitter_.maxVelocity = parameters_.Get<Vector3>("MaxVelocity", baseEmitter_.maxVelocity);
		// 回転
		baseEmitter_.isBillboard = parameters_.Get<bool>("Billboard", baseEmitter_.isBillboard);
		baseEmitter_.minRotate = parameters_.Get<Vector3>("MinRotate", baseEmitter_.minRotate);
		baseEmitter_.maxRotate = parameters_.Get<Vector3>("MaxRotate", baseEmitter_.maxRotate);
		baseEmitter_.minRotateSpeed = parameters_.Get<Vector3>("MinRotateSpeed", baseEmitter_.minRotateSpeed);
		baseEmitter_.maxRotateSpeed = parameters_.Get<Vector3>("MaxRotateSpeed", baseEmitter_.maxRotateSpeed);

		/* ==================== Emitterの固定値 ==================== */

		// サイズ
		baseEmitter_.emitterSize.min = parameters_.Get<Vector3>("MinSize", baseEmitter_.emitterSize.min);
		baseEmitter_.emitterSize.max = parameters_.Get<Vector3>("MaxSize", baseEmitter_.emitterSize.max);
		// Transform
		baseEmitter_.transform.translation = parameters_.Get<Vector3>("Position", baseEmitter_.transform.translation);
		// Rotate,Positionを固定するか
		baseEmitter_.isLock = parameters_.Get<bool>("Look", baseEmitter_.isLock);
	}
}

void ParticleEditor::Update()
{
	ImGui::Separator();

	ImGui::PushItemWidth(150);
	if (ImGui::TreeNode("Particle/Emitter Parameters")) {

		/* ==================== 選択できるテクスチャ ==================== */

		auto items = TextureManager::GetInstance()->GetTextures();
		// テクスチャを選択
		if (ImGui::BeginCombo("Textrue", items[textureIndex_].c_str())) {
			for (int i = 0; i < items.size(); ++i) {
				const bool is_selected = (textureIndex_ == i);
				if (ImGui::Selectable(items[i].c_str(), is_selected)) { textureIndex_ = i; }
				if (is_selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}
		baseEmitter_.texture = items[textureIndex_];

		/* ==================== 選択できるモデル ==================== */

		items.clear();
		items = { "Plane","Ring" };
		// ブレンドを選択
		if (ImGui::BeginCombo("Model", items[modelIndex_].c_str())) {
			for (int i = 0; i < items.size(); ++i) {
				const bool is_selected = (modelIndex_ == i);
				if (ImGui::Selectable(items[i].c_str(), is_selected)) { modelIndex_ = i; }
				if (is_selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}
		baseEmitter_.model_ = modelIndex_;

		/* ==================== 選択できるブレンド ==================== */

		items.clear();
		items = { "None","Normal","Add", "Subtract","Multiply" };
		// ブレンドを選択
		if (ImGui::BeginCombo("BlendMode", items[blendIndex_].c_str())) {
			for (int i = 0; i < items.size(); ++i) {
				const bool is_selected = (blendIndex_ == i);
				if (ImGui::Selectable(items[i].c_str(), is_selected)) { blendIndex_ = i; }
				if (is_selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}
		baseEmitter_.blendMode_ = blendIndex_;

		ImGui::Separator();

		/* ==================== Particle全体の固定値 ==================== */

		if (ImGui::TreeNode("ParticleWhole Parameters")) {

			ImGui::Checkbox("Move", &baseEmitter_.isMoveStart);
			// 発生数
			int emitCount = static_cast<int>(baseEmitter_.count);
			ImGui::DragInt("Count", &emitCount, 1.0f, 0, 1024);
			baseEmitter_.count = static_cast<uint32_t>(emitCount);
			// 発生までの時間
			ImGui::DragFloat("FrequencyTime", &baseEmitter_.frequency, 0.01f);
			// 発生してから消えるまでの時間
			ImGui::DragFloat("LifeTime", &baseEmitter_.lifeTime, 0.01f);
			ImGui::Separator();
			// パーティクルに掛かる加速度
			ImGui::Checkbox("AcceleField", &baseEmitter_.isFieldStart);
			ImGui::DragFloat3("Acceleration", &baseEmitter_.acceleration.x, 0.01f);
			ImGui::DragFloat3("MinAcceleField", &baseEmitter_.minAccelerField.x, 0.01f, -256.0f, 0.0f);
			ImGui::DragFloat3("MaxAcceleField", &baseEmitter_.maxAccelerField.x, 0.01f, 0.0f, 256.0f);
			ImGui::Separator();
			// パーティクルが反射するY軸
			ImGui::Checkbox("Reflect", &baseEmitter_.isReflect);
			ImGui::DragFloat("ReflectY", &baseEmitter_.reflectY, 0.01f);

			ImGui::TreePop();
		}
		ImGui::Separator();

		/* ==================== Particle1つのランダム値 ==================== */

		if (ImGui::TreeNode("ParticleOneByOne Parameters")) {

			// カラー
			ImGui::ColorEdit3("Color", &baseEmitter_.color.x);
			ImGui::DragFloat3("uvTranslation", &baseEmitter_.uvTranslation.x, 0.01f);
			ImGui::Separator();
			// スケール
			ImGui::DragFloat3("MinScale", &baseEmitter_.minScale.x, 0.01f);
			ImGui::DragFloat3("MaxScale", &baseEmitter_.maxScale.x, 0.01f);
			ImGui::Checkbox("ChangeScale", &baseEmitter_.isChangeScale);
			ImGui::DragFloat3("EndScale", &baseEmitter_.endScale.x, 0.01f);
			ImGui::Separator();
			// 速度
			ImGui::DragFloat3("MinVelocity", &baseEmitter_.minVelocity.x, 0.01f);
			ImGui::DragFloat3("MaxVelocity", &baseEmitter_.maxVelocity.x, 0.01f);
			ImGui::Separator();
			// 回転
			ImGui::Checkbox("Billboard", &baseEmitter_.isBillboard);
			ImGui::DragFloat3("MinRotate", &baseEmitter_.minRotate.x, 0.01f);
			ImGui::DragFloat3("MaxRotate", &baseEmitter_.maxRotate.x, 0.01f);
			ImGui::DragFloat3("MinRotateSpeed", &baseEmitter_.minRotateSpeed.x, 0.01f);
			ImGui::DragFloat3("MaxRotateSpeed", &baseEmitter_.maxRotateSpeed.x, 0.01f);

			ImGui::TreePop();
		}
		ImGui::Separator();

		/* ==================== Emitterの固定値 ==================== */

		if (ImGui::TreeNode("EmitterWhole Parameters")) {

			// サイズ
			ImGui::DragFloat3("MinSize", &baseEmitter_.emitterSize.min.x, 0.01f, -256.0f, 0.0f);
			ImGui::DragFloat3("MaxSize", &baseEmitter_.emitterSize.max.x, 0.01f, 0.0f, 256.0f);
			// Transform
			ImGui::Checkbox("Lock", &baseEmitter_.isLock);
			ImGui::DragFloat3("Position", &baseEmitter_.transform.translation.x, 0.01f);

			ImGui::TreePop();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}

	/* ==================== セーブ ==================== */

	if (ImGui::Button("Save")) {
		Save();
	}
}

void ParticleEditor::Save()
{
	/* ==================== Particle全体の固定値 ==================== */

	// Texture
	parameters_.Set("Texture", baseEmitter_.texture);
	// BlendMode
	parameters_.Set("Model", static_cast<int>(baseEmitter_.model_));
	// BlendMode
	parameters_.Set("BlendMode", static_cast<int>(baseEmitter_.blendMode_));
	// 発生数
	parameters_.Set("EmitCount", static_cast<int>(baseEmitter_.count));
	// 発生までの時間
	parameters_.Set("Move", baseEmitter_.isMoveStart);
	parameters_.Set("FrequencyTime", baseEmitter_.frequency);
	// 発生してから消えるまでの時間
	parameters_.Set("LifeTime", baseEmitter_.lifeTime);
	// 加速度
	parameters_.Set("Acceleration", baseEmitter_.acceleration);
	// 加速度が掛かるフィールドのスケール
	parameters_.Set("AcceleField", baseEmitter_.isFieldStart);
	parameters_.Set("MinAcceleField", baseEmitter_.minAccelerField);
	parameters_.Set("MaxAcceleField", baseEmitter_.maxAccelerField);
	// パーティクルが反射するY軸
	parameters_.Set("Reflect", baseEmitter_.isReflect);
	parameters_.Set("ReflectY", baseEmitter_.reflectY);

	/* ==================== Particle1つのランダム値 ==================== */

	// カラー
	parameters_.Set("Color", baseEmitter_.color);
	parameters_.Set("uvTranslation", baseEmitter_.uvTranslation);
	// スケール
	parameters_.Set("MinScale", baseEmitter_.minScale);
	parameters_.Set("MaxScale", baseEmitter_.maxScale);
	parameters_.Set("ChangeScale", baseEmitter_.isChangeScale);
	parameters_.Set("EndScale", baseEmitter_.endScale);
	// 速度
	parameters_.Set("MinVelocity", baseEmitter_.minVelocity);
	parameters_.Set("MaxVelocity", baseEmitter_.maxVelocity);
	// 回転
	parameters_.Set("Billboard", baseEmitter_.isBillboard);
	parameters_.Set("MinRotate", baseEmitter_.minRotate);
	parameters_.Set("MaxRotate", baseEmitter_.maxRotate);
	parameters_.Set("MinRotateSpeed", baseEmitter_.minRotateSpeed);
	parameters_.Set("MaxRotateSpeed", baseEmitter_.maxRotateSpeed);

	/* ==================== Emitterの固定値 ==================== */

	// サイズ
	parameters_.Set("MinSize", baseEmitter_.emitterSize.min);
	parameters_.Set("MaxSize", baseEmitter_.emitterSize.max);
	// Transform
	parameters_.Set("Position", baseEmitter_.transform.translation);
	// Positionを固定するか
	parameters_.Set("Look", baseEmitter_.isLock);

	parameters_.Save();
}
