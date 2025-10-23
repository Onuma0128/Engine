#pragma once

class DirectXEngine;
class WinApp;

/// <summary>
/// ImGui管理クラス
/// </summary>
class ImGuiManager
{
public:

	// デストラクタ
	~ImGuiManager();

	// 初期化
	void Initialize(DirectXEngine* dxEngine, WinApp* winApp);

	// 描画開始
	void Begin();

	// 描画
	void Draw();

	// 描画終了
	void End();

private:

	DirectXEngine* dxEngine_ = nullptr;
	
	WinApp* winApp_ = nullptr;
};