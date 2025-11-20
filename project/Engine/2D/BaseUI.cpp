#include "BaseUI.h"

#include <numbers>

#include "imgui.h"

#include "TextureManager.h"
#include "DeltaTimer.h"
#include "Easing.h"

void BaseUI::Init(const std::string uiName, const std::string biginName)
{
	parameters_.filename = uiName;
	parameters_.biginName = biginName;
	json_.Init(uiName);

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
		// Color
		json_.Set("Color", Vector4{ 1.0f,1.0f,1.0f,1.0f });
		parameters_.color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
		// Animation
		json_.Set("IsAnimation", false);
		json_.Set("AnimationTime", 1.0f);
		json_.Set("Anima_Size", Vector2{ 1.0f,1.0f });
		json_.Set("Anima_Rotate", 0.0f);
		json_.Set("Anima_Position", Vector2{ 0.0f,0.0f });
		parameters_.animaTransform.size = Vector2{ 1.0f,1.0f };
		parameters_.animationTime = 1.0f;
		// Color
		json_.Set("Anima_Color", Vector4{ 1.0f,1.0f,1.0f,1.0f });
		parameters_.animaColor = Vector4{ 1.0f,1.0f,1.0f,1.0f };
		// Easing
		json_.Set("InEasingType", 0);
		json_.Set("OutEasingType", 0);
		parameters_.inEasingType = 0;
		parameters_.outEasingType = 0;
		// Interval
		json_.Set("FadeInInterval", 0.0f);
		json_.Set("FadeOutInterval", 0.0f);

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
		// Color
		parameters_.color = json_.Get<Vector4>("Color", Vector4{ 1.0f,1.0f,1.0f,1.0f });
		parameters_.animaColor = json_.Get<Vector4>("Anima_Color", Vector4{ 1.0f,1.0f,1.0f,1.0f });
		// Animation
		parameters_.isAnimation = json_.Get<bool>("IsAnimation", false);
		parameters_.animationTime = json_.Get<float>("AnimationTime", 1.0f);
		parameters_.animaTransform.size = json_.Get<Vector2>("Anima_Size", Vector2{ 1.0f,1.0f });
		parameters_.animaTransform.rotate = json_.Get<float>("Anima_Rotate", 0.0f);
		parameters_.animaTransform.position = json_.Get<Vector2>("Anima_Position", Vector2{ 0.0f,0.0f });
		// Easing
		parameters_.inEasingType = json_.Get<int>("InEasingType", 0);
		parameters_.outEasingType = json_.Get<int>("OutEasingType", 0);
		const auto& names = Easing::GetEaseTypeNames();
		if (parameters_.inEasingType < 0 || parameters_.inEasingType >= static_cast<int>(names.size())) {
			parameters_.inEasingType = 0;
		}
		if (parameters_.outEasingType < 0 || parameters_.outEasingType >= static_cast<int>(names.size())) {
			parameters_.outEasingType = 0;
		}
		// Interval
		parameters_.fadeInInterval = json_.Get<float>("FadeInInterval", 0.0f);
		parameters_.fadeOutInterval = json_.Get<float>("FadeOutInterval", 0.0f);
	}
	// 選択できるテクスチャ
	auto items = TextureManager::GetInstance()->GetTextures();
	// テクスチャを検索
	for (int i = 0; i < items.size(); ++i) { if (parameters_.texture == items[i]) { textureIndex_ = i; } }

	ui_ = std::make_unique<Sprite>();
	ui_->Initialize(parameters_.texture);
	ui_->SetAnchorPoint(parameters_.anchorPoint);
	ui_->GetTransform() = parameters_.transform;

	playAnimationTimer_ = 0.0f;
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
#ifdef ENABLE_EDITOR

	ImGui::Begin(parameters_.biginName.c_str());
	if (ImGui::TreeNode(parameters_.filename.c_str())) {
		ImGui::Separator();
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
		if (!isPlayAnimation_ && playAnimationTimer_ <= 0.0f) {
			ui_->GetTransform() = parameters_.transform;
		}
		// Color
		ImGui::ColorEdit4("Color", &parameters_.color.x);
		// Animation
		ImGui::Checkbox("IsAnimation", &parameters_.isAnimation);
		if (parameters_.isAnimation) {
			ImGui::DragFloat("AnimationTime", &parameters_.animationTime, 0.01f);
			const auto& names = Easing::GetEaseTypeNames();
			int idx = std::clamp(parameters_.inEasingType, 0, static_cast<int>(names.size()) - 1);
			if (ImGui::BeginCombo("InEasingType", names[idx])) {
				for (int i = 0; i < static_cast<int>(names.size()); ++i) {
					bool sel = (parameters_.inEasingType == i);
					if (ImGui::Selectable(names[i], sel)) {
						parameters_.inEasingType = i;
					}
					if (sel) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			idx = std::clamp(parameters_.outEasingType, 0, static_cast<int>(names.size() - 1));
			if (ImGui::BeginCombo("OutEasingType", names[idx])) {
				for (int i = 0; i < static_cast<int>(names.size()); ++i) {
					bool sel = (parameters_.outEasingType == i);
					if (ImGui::Selectable(names[i], sel)) {
						parameters_.outEasingType = i;
					}
					if (sel) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::DragFloat("FadeInInterval", &parameters_.fadeInInterval, 0.01f);
			ImGui::DragFloat("FadeOutInterval", &parameters_.fadeOutInterval, 0.01f);
			if (ImGui::Button("FadeIn")) { FadeIn(); }
			if (ImGui::Button("FadeOut")) { FadeOut(); }
			ImGui::DragFloat2("Anima_Size", &parameters_.animaTransform.size.x, 0.5f);
			ImGui::DragFloat("Anima_Rotate", &parameters_.animaTransform.rotate, 0.01f);
			ImGui::DragFloat2("Anima_Position", &parameters_.animaTransform.position.x, 0.5f);
			if (!isPlayAnimation_ && playAnimationTimer_ >= 1.0f) {
				ui_->GetTransform() = parameters_.animaTransform;
			}
			// Color
			ImGui::ColorEdit4("Anima_Color", &parameters_.animaColor.x);
		}

		if (ImGui::Button("Save")) {
			Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();

#endif // ENABLE_EDITOR
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
	// Color
	json_.Set("Color", parameters_.color);
	json_.Set("Anima_Color", parameters_.animaColor);
	// Animation
	json_.Set("IsAnimation", parameters_.isAnimation);
	json_.Set("AnimationTime", parameters_.animationTime);
	json_.Set("Anima_Size", parameters_.animaTransform.size);
	json_.Set("Anima_Rotate", parameters_.animaTransform.rotate);
	json_.Set("Anima_Position", parameters_.animaTransform.position);
	json_.Set("InEasingType", parameters_.inEasingType);
	json_.Set("OutEasingType", parameters_.outEasingType);
	json_.Set("FadeInInterval", parameters_.fadeInInterval);
	json_.Set("FadeOutInterval", parameters_.fadeOutInterval);
	json_.Save();
}

void BaseUI::FadeIn()
{
	isPlayAnimation_ = true;
	reversePlayBack_ = false;
	playAnimationTimer_ -= parameters_.fadeInInterval;
}

void BaseUI::FadeOut()
{
	isPlayAnimation_ = true;
	reversePlayBack_ = true;
	playAnimationTimer_ += parameters_.fadeOutInterval;
}

void BaseUI::Blinking()
{
	// フレームを進める
	blinkingTime_ += DeltaTimer::GetDeltaTime() * 5.0f;
	if (blinkingTime_ > std::numbers::pi_v<float> * 2.0f) {
		blinkingTime_ -= std::numbers::pi_v<float> * 2.0f;
	}

	// alphaを0.5～1.0でループさせる
	float alpha = 0.75f + 0.25f * std::cos(blinkingTime_);
	// カラーをセットする
	ui_->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, alpha });
}

void BaseUI::Reset()
{
	blinkingTime_ = 0.0f;
	ui_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
}

void BaseUI::UI_Animation()
{
	// Animationが有効なら再生する
	if (isPlayAnimation_) {
		// 逆再生なら
		if (reversePlayBack_) {
			playAnimationTimer_ -= (DeltaTimer::GetDeltaTime() / parameters_.animationTime);
		// 逆再生じゃ無いなら
		} else {
			playAnimationTimer_ += (DeltaTimer::GetDeltaTime() / parameters_.animationTime);
		}
		float t = std::clamp(playAnimationTimer_, 0.0f, 1.0f);
		//playAnimationTimer_ = t;
		// 逆再生なら
		if (reversePlayBack_) {
			t = Easing::Apply(Easing::FromInt(parameters_.outEasingType), t);
			if (t == 0.0f) { isPlayAnimation_ = false; }
		// 逆再生じゃ無いなら
		} else {
			t = Easing::Apply(Easing::FromInt(parameters_.inEasingType), t);
			if (t == 1.0f) { isPlayAnimation_ = false; }
		}

		// TransformをAnimationさせていく
		ui_->GetTransform().size = Vector2::EaseLerp(parameters_.transform.size, parameters_.animaTransform.size, t);
		ui_->GetTransform().rotate = (1.0f - t) * parameters_.transform.rotate + t * parameters_.animaTransform.rotate;
		ui_->GetTransform().position = Vector2::EaseLerp(parameters_.transform.position, parameters_.animaTransform.position, t);
		ui_->SetColor(Vector4::Lerp(parameters_.color,parameters_.animaColor,t));
	}
}
