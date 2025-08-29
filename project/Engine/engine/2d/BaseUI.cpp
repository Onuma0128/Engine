#include "BaseUI.h"

#include "imgui.h"

#include "TextureManager.h"

void BaseUI::Init(const std::string UI_Name)
{
	parameters_.filename = UI_Name;
	json_.Init(UI_Name);

	// ロードするファイルが無ければ
	if (!json_.Load()) {
		// Texture
		json_.Set("Texture", "white1x1.png");
		parameters_.texture = "white1x1.png";

		// AnchorPoint
		json_.Set("AnchorPoint", Vector2{ 0.0f,0.0f });
		// Transform
		json_.Set("Size", Vector2{ 1.0f,1.0f });
		json_.Set("Rotate", 0.0f);
		json_.Set("Position", Vector2{ 0.0f,0.0f });
		parameters_.transform.size = Vector2{ 1.0f,1.0f };

	// ロードするファイルが有れば
	} else {
		// Texture
		parameters_.texture = json_.Get<std::string>("Texture", "white1x1.png");

		// AnchorPoint
		parameters_.anchorPoint = json_.Get<Vector2>("AnchorPoint", Vector2{ 0.0f,0.0f });
		// Transform
		parameters_.transform.size = json_.Get<Vector2>("Size", Vector2{ 1.0f,1.0f });
		parameters_.transform.rotate = json_.Get<float>("Rotate", 0.0f);
		parameters_.transform.position = json_.Get<Vector2>("Position", Vector2{ 0.0f,0.0f });
	}
	// 選択できるテクスチャ
	auto items = TextureManager::GetInstance()->GetTextures();
	// テクスチャを検索
	for (int i = 0; i < items.size(); ++i) { if (parameters_.texture == items[i]) { textureIndex_ = i; } }

	ui_ = std::make_unique<Sprite>();
	ui_->Initialize(parameters_.texture);
	ui_->SetAnchorPoint(parameters_.anchorPoint);
	ui_->GetTransform() = parameters_.transform;
}

void BaseUI::Update()
{
	ui_->Update();
}

void BaseUI::Draw()
{
	ui_->Draw();
}

void BaseUI::DrawImGui()
{
#ifdef _DEBUG

	ImGui::Begin(parameters_.filename.c_str());
	ImGui::PushItemWidth(150);

	// テクスチャを選択
	int index = textureIndex_;
	auto items = TextureManager::GetInstance()->GetTextures();
	if (ImGui::BeginCombo("Textrue", items[textureIndex_].c_str())) {
		for (int i = 0; i < items.size(); ++i) {
			const bool is_selected = (textureIndex_ == i);
			if (ImGui::Selectable(items[i].c_str(), is_selected)) { textureIndex_ = i; }
			if (is_selected) { ImGui::SetItemDefaultFocus(); }
		}
		ImGui::EndCombo();
	}
	parameters_.texture = items[textureIndex_];
	if (index != textureIndex_) { ui_->SetTexture(parameters_.texture); }

	// AnchorPoint
	ImGui::DragFloat2("AnchorPoint", &parameters_.anchorPoint.x, 0.01f);
	ui_->SetAnchorPoint(parameters_.anchorPoint);
	// Transform
	ImGui::DragFloat2("Size", &parameters_.transform.size.x, 0.5f);
	ImGui::DragFloat("Rotate", &parameters_.transform.rotate, 0.01f);
	ImGui::DragFloat2("Position", &parameters_.transform.position.x, 0.5f);
	ui_->GetTransform() = parameters_.transform;

	if (ImGui::Button("Save")) {
		Save();
	}

	ImGui::End();

#endif // _DEBUG
}

void BaseUI::Save()
{
	// Texture
	json_.Set("Texture", parameters_.texture);
	// AnchorPoint
	json_.Set("AnchorPoint", parameters_.anchorPoint);
	// Transform
	json_.Set("Size", parameters_.transform.size);
	json_.Set("Rotate", parameters_.transform.rotate);
	json_.Set("Position", parameters_.transform.position);
	json_.Save();
}
