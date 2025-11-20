#pragma once

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager
{
private:

	// シングルトンインスタンス
	static SceneManager* instance_;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

public:

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
	BaseScene* scene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;
};