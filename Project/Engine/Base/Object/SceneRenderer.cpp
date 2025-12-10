#include "SceneRenderer.h"

#include "DirectXEngine.h"
#include "ParticleManager.h"
#include "ModelInstanceRenderer.h"

void SceneRenderer::Finalize()
{
	objects_.clear();
	sprites_.clear();
	lines_.clear();
	primitiveDrawrs_.clear();
	animations_.clear();
}

void SceneRenderer::AllDraw()
{
	/// ============================== Sprite ============================== ///
	for (auto& sprite : sprites_) {
		if (!sprite->GetRenderOptions().enabled || !sprite->GetRenderOptions().offscreen) { continue; }
		sprite->Draw();
	}
	/// ============================== PrimitiveDrawr ============================== ///
	for (auto& primitive : primitiveDrawrs_) {
		if (!primitive->GetRenderOptions().enabled || !primitive->GetRenderOptions().offscreen) { continue; }
		if (primitive->GetPrimitiveType() == PrimitiveType::kNone) { primitive->Draw(); }
		else { primitive->TypeDraw(); }
	}
}

void SceneRenderer::OutAllDraw()
{
	/// ============================== Sprite ============================== ///
	for (auto& sprite : sprites_) {
		if (!sprite->GetRenderOptions().enabled || sprite->GetRenderOptions().offscreen) { continue; }
		sprite->Draw();
	}

	/// ============================== PrimitiveDrawr ============================== ///
	for (auto& primitive : primitiveDrawrs_) {
		if (!primitive->GetRenderOptions().enabled || primitive->GetRenderOptions().offscreen) { continue; }
		if (primitive->GetPrimitiveType() == PrimitiveType::kNone) { primitive->Draw(); }
		else { primitive->TypeDraw(); }
	}
}
