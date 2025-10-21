#include "MyGame.h"

#include "SceneFactory.h"

#include "SceneManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "DeltaTimer.h"
#include "ParticleManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("Title");

	DeltaTimer::Initialize();
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
	DeltaTimer::Update();

	// 当たり判定
	directXEngine_->GetCollisionMgr()->CheckAllCollisions();

	// シーンの更新
	SceneManager::GetInstance()->Update();

	// 登録済みのLineを更新
	directXEngine_->GetLineRenderer()->Update();

	// カメラの更新
	CameraManager::GetInstance()->Debug_ImGui();
	CameraManager::GetInstance()->Update();

	// ライトの更新
	LightManager::GetInstance()->Debug_ImGui();
	LightManager::GetInstance()->Update();

	// ImGui受付終了
	imGuiManager_->End();
}

void MyGame::Draw()
{
	// 描画前の処理
	directXEngine_->PreDraw();
	// offscreen描画
	directXEngine_->GetModelRenderer()->AllDraw();
	directXEngine_->GetSceneRenderer()->AllDraw();
	// パーティクルの描画
	//directXEngine_->RenderTexturePreDraw();
	ParticleManager::GetInstance()->Draw();
	// outlineMask処理
	directXEngine_->GetPostEffectMgr()->BeginOutlineMaskPass();
	directXEngine_->GetModelRenderer()->AllDrawOutlineMask();
	directXEngine_->GetPostEffectMgr()->EndOutlineMaskPass();
	// offscreen描画終了
	directXEngine_->RenderPost();
	// postEffectの書き込み描画
	directXEngine_->SetPostEffectDraw(PostEffectType::Grayscale);
	directXEngine_->SetPostEffectDraw(PostEffectType::Vignette);
	directXEngine_->SetPostEffectDraw(PostEffectType::OutLine);
	// 書き込んだoffscreenを描画
	directXEngine_->RenderTextureDraw();

	// offscreenを掛けたくない描画
	directXEngine_->GetSceneRenderer()->OutAllDraw();
	// Lineの描画
	directXEngine_->GetLineRenderer()->Draws();
	SceneManager::GetInstance()->Draw();


	// ImGuiの描画
	imGuiManager_->Draw();

	// 描画後の処理
	directXEngine_->PostDraw();
}
