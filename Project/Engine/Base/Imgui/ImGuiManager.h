#pragma once

namespace NumaEngine { class DirectXEngine; class WinApp; }

/// <summary>
/// ImGui管理クラス
/// </summary>
namespace NumaEngine {
class ImGuiManager
{
public:

	// デストラクタ
	~ImGuiManager();

	// 初期化
    void Initialize(NumaEngine::DirectXEngine* dxEngine, NumaEngine::WinApp* winApp);

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
} // namespace NumaEngine
