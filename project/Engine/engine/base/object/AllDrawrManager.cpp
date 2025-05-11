#include "AllDrawrManager.h"

#include "ParticleManager.h"

void AllDrawrManager::Finalize()
{
	objects_.clear();
	sprites_.clear();
	lines_.clear();
	primitiveDrawrs_.clear();
	animations_.clear();
}

void AllDrawrManager::AllDraw()
{
	/// ============================== Object3d ============================== ///
	for (auto& object : objects_) {
		if (!object->GetRenderOptions().enabled || !object->GetRenderOptions().offscreen) { continue; }
		object->Draw();
	}
	/// ============================== Sprite ============================== ///
	for (auto& sprite : sprites_) {
		if (!sprite->GetRenderOptions().enabled || !sprite->GetRenderOptions().offscreen) { continue; }
		sprite->Draw();
	}
	/// ============================== Line3d ============================== ///
	for (auto& line : lines_) {
		if (!line->GetRenderOptions().enabled || !line->GetRenderOptions().offscreen) { continue; }
		if (line->GetIsMultiple()) { line->Draws(); }
		else { line->Draw(); }
	}
	/// ============================== PrimitiveDrawr ============================== ///
	for (auto& primitive : primitiveDrawrs_) {
		if (!primitive->GetRenderOptions().enabled || !primitive->GetRenderOptions().offscreen) { continue; }
		if (primitive->GetPrimitiveType() == PrimitiveType::None) { primitive->Draw(); }
		else { primitive->TypeDraw(); }
	}
	/// ============================== Animation ============================== ///
	for (auto& animation : animations_) {
		if (!animation->GetRenderOptions().enabled || !animation->GetRenderOptions().offscreen) { continue; }
		animation->Draw();
	}

	ParticleManager::GetInstance()->Draw();
}

void AllDrawrManager::OutAllDraw()
{
	/// ============================== Object3d ============================== ///
	for (auto& object : objects_) {
		if (!object->GetRenderOptions().enabled || object->GetRenderOptions().offscreen) { continue; }
		object->Draw();
	}
	/// ============================== Sprite ============================== ///
	for (auto& sprite : sprites_) {
		if (!sprite->GetRenderOptions().enabled || sprite->GetRenderOptions().offscreen) { continue; }
		sprite->Draw();
	}
	/// ============================== Line3d ============================== ///
	for (auto& line : lines_) {
		if (!line->GetRenderOptions().enabled || line->GetRenderOptions().offscreen) { continue; }
		if (line->GetIsMultiple()) { line->Draws(); }
		else { line->Draw(); }
	}
	/// ============================== PrimitiveDrawr ============================== ///
	for (auto& primitive : primitiveDrawrs_) {
		if (!primitive->GetRenderOptions().enabled || primitive->GetRenderOptions().offscreen) { continue; }
		if (primitive->GetPrimitiveType() == PrimitiveType::None) { primitive->Draw(); }
		else { primitive->TypeDraw(); }
	}
	/// ============================== Animation ============================== ///
	for (auto& animation : animations_) {
		if (!animation->GetRenderOptions().enabled || animation->GetRenderOptions().offscreen) { continue; }
		animation->Draw();
	}
}
