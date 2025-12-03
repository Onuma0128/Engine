#include "SceneManager.h"
#include <cassert>

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "CameraManager.h"
#include "ParticleManager.h"

std::unique_ptr<SceneManager> SceneManager::instance_ = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = std::make_unique<SceneManager>();
	}
	return instance_.get();
}

void SceneManager::Update()
{
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
			CameraManager::GetInstance()->Clear();
			ParticleManager::GetInstance()->Clear();
			DirectXEngine::GetSceneRenderer()->Finalize();
			DirectXEngine::GetModelRenderer()->Finalize();
			DirectXEngine::GetLineRenderer()->Finalize();
			DirectXEngine::GetCollisionMgr()->ClearCollider();
			scene_->Finalize();
		}

		// シーン切り替え
		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;
		// 次シーンを初期化する
		scene_->Initialize();
		DirectXEngine::GetModelRenderer()->Initialize();
	}

	// 実行中のシーンの更新
	scene_->Update();
}

void SceneManager::Draw()
{
	// 実行中のシーンの描画
	scene_->Draw();
}

void SceneManager::Finalize()
{
	scene_->Finalize();

	instance_ = nullptr;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
