#include "BaseUI.h"

#include "imgui.h"

#include "TextureManager.h"
#include "DeltaTimer.h"
#include "Easing.h"

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
		// Animation
		json_.Set("IsAnimation", false);
		json_.Set("AnimationTime", 0.0f);
		json_.Set("Anima_Size", Vector2{ 1.0f,1.0f });
		json_.Set("Anima_Rotate", 0.0f);
		json_.Set("Anima_Position", Vector2{ 0.0f,0.0f });
		parameters_.animaTransform.size = Vector2{ 1.0f,1.0f };
		// Easing
		json_.Set("EasingType", 0);
		parameters_.easingType = 0;

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
		// Animation
		parameters_.isAnimation = json_.Get<bool>("IsAnimation", false);
		parameters_.animationTime = json_.Get<float>("AnimationTime", 0.0f);
		parameters_.animaTransform.size = json_.Get<Vector2>("Anima_Size", Vector2{ 1.0f,1.0f });
		parameters_.animaTransform.rotate = json_.Get<float>("Anima_Rotate", 0.0f);
		parameters_.animaTransform.position = json_.Get<Vector2>("Anima_Position", Vector2{ 0.0f,0.0f });
		// Easing
		parameters_.easingType = json_.Get<int>("EasingType", 0);
		const auto& names = Easing::GetEaseTypeNames();
		if (parameters_.easingType < 0 || parameters_.easingType >= (int)names.size()) {
			parameters_.easingType = 0;
		}
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
	UI_Animation();

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
	if (!isPlayAnimation_ && playAnimationTimer_ == 0.0f) {
		ui_->GetTransform() = parameters_.transform;
	}
	// Animation
	ImGui::Checkbox("IsAnimation", &parameters_.isAnimation);
	ImGui::DragFloat("AnimationTime", &parameters_.animationTime, 0.01f);
	if (parameters_.isAnimation) {
		const auto& names = Easing::GetEaseTypeNames();
		int idx = std::clamp(parameters_.easingType, 0, (int)names.size() - 1);
		if (ImGui::BeginCombo("EasingType", names[idx])) {
			for (int i = 0; i < (int)names.size(); ++i) {
				bool sel = (parameters_.easingType == i);
				if (ImGui::Selectable(names[i], sel)) {
					parameters_.easingType = i;
				}
				if (sel) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("FadeIn")) { FadeIn(); }
		if (ImGui::Button("FadeOut")) { FadeOut(); }
	}
	ImGui::DragFloat2("Anima_Size", &parameters_.animaTransform.size.x, 0.5f);
	ImGui::DragFloat("Anima_Rotate", &parameters_.animaTransform.rotate, 0.01f);
	ImGui::DragFloat2("Anima_Position", &parameters_.animaTransform.position.x, 0.5f);

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
	// Animation
	json_.Set("IsAnimation", parameters_.isAnimation);
	json_.Set("AnimationTime", parameters_.animationTime);
	json_.Set("Anima_Size", parameters_.animaTransform.size);
	json_.Set("Anima_Rotate", parameters_.animaTransform.rotate);
	json_.Set("Anima_Position", parameters_.animaTransform.position);
	json_.Set("EasingType", parameters_.easingType);
	json_.Save();
}

void BaseUI::FadeIn()
{
	isPlayAnimation_ = true;
	reversePlayBack_ = false;
}

void BaseUI::FadeOut()
{
	isPlayAnimation_ = true;
	reversePlayBack_ = true;
}

void BaseUI::UI_Animation()
{
	// Animationが有効なら再生する
	if (isPlayAnimation_) {
		// 逆再生なら
		if (reversePlayBack_) {
			playAnimationTimer_ -= DeltaTimer::GetDeltaTime() / parameters_.animationTime;
		// 逆再生じゃ無いなら
		} else {
			playAnimationTimer_ += DeltaTimer::GetDeltaTime() / parameters_.animationTime;
		}
		float t = std::clamp(playAnimationTimer_, 0.0f, 1.0f);
		playAnimationTimer_ = t;
		float et = Easing::Apply(Easing::FromInt(parameters_.easingType), t);

		// TransformをAnimationさせていく
		ui_->GetTransform().size = Vector2::EaseLerp(parameters_.transform.size, parameters_.animaTransform.size, et);
		ui_->GetTransform().rotate = (1.0f - et) * parameters_.transform.rotate + et * parameters_.animaTransform.rotate;
		ui_->GetTransform().position = Vector2::EaseLerp(parameters_.transform.position, parameters_.animaTransform.position, et);

		if (t == 0.0f || t == 1.0f) {
			isPlayAnimation_ = false;
		}
	}
}
