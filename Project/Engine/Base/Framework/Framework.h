#pragma once
#include <memory>

#include "WinApp.h"
#include "DirectXEngine.h"
#include "ImGuiManager.h"

#include "AbstractSceneFactory.h"

/// <summary>
/// フレームワーククラス
/// </summary>
class Framework
{
public:

	// デストラクタ
	virtual ~Framework() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw() = 0;

	// 終了要求の取得
	virtual bool IsEndRequst() { return endRequst_; }

	// ファイル読み込み
	void LoadFiles();

	// 実行
	void Run();

protected:

	// ゲーム終了フラグ
	bool endRequst_ = false;

	std::unique_ptr<WinApp> winApp_ = nullptr;
	std::unique_ptr<DirectXEngine> directXEngine_ = nullptr;

	std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;

	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;

};