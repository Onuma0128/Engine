#include "SceneManager.h"
#include <cassert>

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "CameraManager.h"
#include "ParticleManager.h"
#include "LineInstanceRenderer.h"
#include "ModelInstanceRenderer.h"
#include "CollisionManager.h"

std::unique_ptr<NumaEngine::SceneManager> NumaEngine::SceneManager::instance_ = nullptr;

NumaEngine::SceneManager* NumaEngine::SceneManager::GetInstance()
{
    if (instance_ == nullptr) {
		instance_ = std::make_unique<NumaEngine::SceneManager>();
	}
	return instance_.get();
}

void NumaEngine::SceneManager::Update()
{
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
            NumaEngine::CameraManager::GetInstance()->Clear();
			NumaEngine::ParticleManager::GetInstance()->Clear();
			NumaEngine::DirectXEngine::GetSceneRenderer()->Finalize();
			NumaEngine::ModelInstanceRenderer::GetInstance()->Finalize();
			NumaEngine::LineInstanceRenderer::GetInstance()->Finalize();
			NumaEngine::CollisionManager::GetInstance()->ClearCollider();
			scene_->Finalize();
		}

		// シーン切り替え
        scene_ = std::move(nextScene_);
		nextScene_ = nullptr;
		// 次シーンを初期化する
		scene_->Initialize();
        NumaEngine::ModelInstanceRenderer::GetInstance()->Initialize();
	}

	// 実行中のシーンの更新
	scene_->Update();
}

void NumaEngine::SceneManager::Draw()
{
	// 実行中のシーンの描画
	scene_->Draw();
}

void NumaEngine::SceneManager::Finalize()
{
	scene_->Finalize();

	instance_ = nullptr;
}

void NumaEngine::SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName);
}

