#include "MyGame.h"

#include "SceneFactory.h"

#include "SceneManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "DeltaTimer.h"

#include "GlobalVariables.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("Title");

	DeltaTimer::Initialize();

	//GlobalVariables::GetInstance()->LoadFiles();
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

	directXEngine_->GetSceneRenderer()->AllDraw();

	directXEngine_->GetPostEffectMgr()->BeginOutlineMaskPass();
	directXEngine_->GetModelRenderer()->AllDrawOutlineMask();
	directXEngine_->GetPostEffectMgr()->EndOutlineMaskPass();

	directXEngine_->SwapChainDrawSet();

	directXEngine_->GetSceneRenderer()->OutAllDraw();

	SceneManager::GetInstance()->Draw();

	// Lineの描画
	//directXEngine_->GetLineRenderer()->Draws();


	// ImGuiの描画
	imGuiManager_->Draw();

	// 描画後の処理
	directXEngine_->PostDraw();
}
