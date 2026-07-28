#include "MyGame.h"

#include "SceneFactory.h"

#include "SrvManager.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelInstanceRenderer.h"
#include "LineInstanceRenderer.h"
#include "ParticleManager.h"
#include "PostEffectManager.h"
#include "CollisionManager.h"
#include "ShadowMap.h"

#include "DeltaTimer.h"

void MyGame::Initialize()
{
	Framework::Initialize();

    sceneFactory_ = std::make_unique<NumaEngine::SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	SceneManager::GetInstance()->ChangeScene("Title");

    NumaEngine::DeltaTimer::Initialize();
}

void MyGame::Finalize()
{
	winApp_->Finalize();

	Framework::Finalize();
}

void MyGame::Update()
{
	// ImGui受付開始
	imGuiManager_->Begin();

	Framework::Update();
	
    // DeltaTimeの更新
	NumaEngine::DeltaTimer::Update();

    // 当たり判定
	NumaEngine::CollisionManager::GetInstance()->CheckAllCollisions();

	// シーンの更新
	SceneManager::GetInstance()->Update();

    // 登録済みのLineを更新
	NumaEngine::LineInstanceRenderer::GetInstance()->Update();

	// ライトの更新
	LightManager::GetInstance()->Debug_ImGui();
	LightManager::GetInstance()->Update();

    // カメラの更新
	NumaEngine::CameraManager::GetInstance()->Debug_ImGui();
	NumaEngine::CameraManager::GetInstance()->Update();

	// ImGui受付終了
	imGuiManager_->End();
}

void MyGame::Draw()
{
	//描画用のDescriptorHeapの設定
	SrvManager::GetInstance()->PreDraw();
	// シャドウマップ深度処理
	ShadowMap::GetInstance()->BeginShadowMapPass();
 NumaEngine::ModelInstanceRenderer::GetInstance()->AllDrawShadowDepth();
	ShadowMap::GetInstance()->EndShadowMapPass();
	// 描画前の処理
	directXEngine_->PreDraw();
	// offscreen描画
	directXEngine_->GetSceneRenderer()->AllDraw();
    NumaEngine::ModelInstanceRenderer::GetInstance()->AllDraw();
	// パーティクルの描画
	//directXEngine_->RenderTexturePreDraw();
	NumaEngine::ParticleManager::GetInstance()->Draw();
	// outlineMask処理
	PostEffectManager::GetInstance()->BeginOutlineMaskPass();
 NumaEngine::ModelInstanceRenderer::GetInstance()->AllDrawOutlineMask();
	PostEffectManager::GetInstance()->EndOutlineMaskPass();
	// offscreen描画終了
	directXEngine_->RenderPost();
	// postEffectの書き込み描画
	directXEngine_->SetPostEffectDraw(PostEffectType::kGrayscale);
	directXEngine_->SetPostEffectDraw(PostEffectType::kVignette);
	directXEngine_->SetPostEffectDraw(PostEffectType::kOutLine);
	// 書き込んだoffscreenを描画
	directXEngine_->RenderTextureDraw();

	// offscreenを掛けたくない描画
	directXEngine_->GetSceneRenderer()->OutAllDraw();
    // Lineの描画
	NumaEngine::LineInstanceRenderer::GetInstance()->Draws();
	SceneManager::GetInstance()->Draw();


	// ImGuiの描画
	imGuiManager_->Draw();

	// 描画後の処理
	directXEngine_->PostDraw();
}
