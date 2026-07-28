#include "ImGuiManager.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "DirectXEngine.h"
#include "WinApp.h"
#include "SrvManager.h"
#include "DescriptorHeap.h"

NumaEngine::ImGuiManager::~ImGuiManager()
{
	// ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void NumaEngine::ImGuiManager::Initialize(NumaEngine::DirectXEngine* dxEngine, NumaEngine::WinApp* winApp)
{
	dxEngine_ = dxEngine;
	winApp_ = winApp;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(
		dxEngine_->GetDevice(),
		static_cast<int>(dxEngine_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		SrvManager::GetInstance()->GetDescriptorHeap(),
		SrvManager::GetInstance()->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		SrvManager::GetInstance()->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart()
	);
}

void NumaEngine::ImGuiManager::Begin()
{
	//ImGuiの開始処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void NumaEngine::ImGuiManager::End()
{
	// ImGuiの内部コマンドを生成
	ImGui::Render();
}

void NumaEngine::ImGuiManager::Draw()
{
#ifdef ENABLE_EDITOR
	// 実際のnommandListのImGuiの描画コマンドを積む
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), NumaEngine::DirectXEngine::GetCommandList());
#endif // ENABLE_EDITOR
}

