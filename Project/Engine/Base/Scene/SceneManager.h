#pragma once

#include <memory>

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager
{
private:

	// シングルトンインスタンス
	static std::unique_ptr<SceneManager> instance_;

public:

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

	// シングルトンインスタンスの取得
	static SceneManager* GetInstance();

public:

	// 初期化
	void Update();

	// 描画
	void Draw();

	// 終了
	void Finalize();

	// 次のシーン予約
	void ChangeScene(const std::string& sceneName);
	// シーンファクトリーのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	// 今のシーン
	std::unique_ptr<BaseScene> scene_ = nullptr;
	// 次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;
};