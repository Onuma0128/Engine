#include "Input.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "WinApp.h"

#include "DeltaTimer.h"

Input* Input::instance_ = nullptr;

Input* Input::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new Input;
	}
	return instance_;
}

void Input::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

void Input::Initialize(WinApp* winApp)
{
	winApp_ = winApp;

	HRESULT hr{};

	// 前回のキー入力を保存
	memcpy(keyPre_, key_, sizeof(key_));

	hr = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput_, nullptr);
	assert(SUCCEEDED(hr));


	// キーボードデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));
	// 入力データ形式のセット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	// 排他制御レベルのセット
	hr = keyboard_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));


	// マウスデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(hr));
	// 入力データ形式のセット
	hr = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));
	// 排他制御レベルのセット
	hr = mouse_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	mouse_->Acquire();
}

void Input::Update()
{
	// 前回のキー入力を保存
	memcpy(keyPre_, key_, sizeof(key_));
	// キーボード情報の取得開始
	keyboard_->Acquire();
	// 全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

	// マウス情報の取得開始
	mouse_->Acquire();
	HRESULT hr = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
	if (FAILED(hr)) {
		// デバイスが一時的に使用できない場合は再取得
		mouse_->Acquire();
	}

	// マウスの移動量とボタンの状態を取得
	mouseDeltaX_ = mouseState_.lX;
	mouseDeltaY_ = mouseState_.lY;
	POINT cursorPos;
	if (GetCursorPos(&cursorPos)) {
		// クライアント座標系に変換
		ScreenToClient(winApp_->GetHwnd(), &cursorPos);
		mousePosX_ = cursorPos.x;
		mousePosY_ = cursorPos.y;
	}

	memcpy(mouseButtonPre_, mouseButton_, sizeof(mouseButton_));
	memcpy(mouseButton_, mouseState_.rgbButtons, sizeof(mouseButton_));

	// XInput の更新
	xInputStatePre_ = xInputState_; // 前回の状態を保存
	ZeroMemory(&xInputState_, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(0, &xInputState_); // コントローラー 0 を取得
	if (result != ERROR_SUCCESS) {
		// コントローラーが接続されていない場合
		ZeroMemory(&xInputState_, sizeof(XINPUT_STATE));
	}

	const bool connected = (result == ERROR_SUCCESS);
	if (!connected) {
		if (rumble_.active) { StopVibration(); }
		return;
	}

	if (rumble_.active) {
		// ▼ スローモーションに追従させる場合
		float dt = DeltaTimer::GetDeltaTime();

		// 現実時間で減らしたい場合

		rumble_.remainSec -= dt;
		if (rumble_.remainSec <= 0.0f) {
			StopVibration();
		}
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	// 指定キーが押していればtrueを返す
	return key_[keyNumber] != 0;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定キーが現在押されていて、前回は押されていなかった場合にtrueを返す
	return key_[keyNumber] && !keyPre_[keyNumber];
}

bool Input::PushMouseButton(int buttonNumber) const
{
	return (mouseButton_[buttonNumber] & 0x80) != 0;
}

bool Input::TriggerMouseButton(int buttonNumber) const
{
	return (mouseButton_[buttonNumber] & 0x80) != 0 && !(mouseButtonPre_[buttonNumber] & 0x80);
}

bool Input::PushGamepadButton(WORD button) const
{
	return (xInputState_.Gamepad.wButtons & button) != 0;
}

bool Input::TriggerGamepadButton(WORD button) const
{
	return (xInputState_.Gamepad.wButtons & button) != 0 &&
		!(xInputStatePre_.Gamepad.wButtons & button);
}

float Input::NormalizeStickValue(SHORT value, SHORT deadzone)
{
	if (abs(value) < deadzone) {
		return 0.0f; // デッドゾーン内は 0
	}
	return static_cast<float>(value) / 32767.0f; // -1.0 ～ 1.0 に正規化
}

void Input::ApplyVibration(float left, float right)
{
	auto clamp01 = [](float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); };
	XINPUT_VIBRATION vib{};
	vib.wLeftMotorSpeed = static_cast<WORD>(clamp01(left) * 65535.0f);
	vib.wRightMotorSpeed = static_cast<WORD>(clamp01(right) * 65535.0f);
	XInputSetState(0, &vib);
}

float Input::GetGamepadLeftStickX() const {
	int deadzone = static_cast<int>(static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * deadzoneScale_);
	return NormalizeStickValue(xInputState_.Gamepad.sThumbLX, deadzone);
}

float Input::GetGamepadLeftStickY() const {
	int deadzone = static_cast<int>(static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * deadzoneScale_);
	return NormalizeStickValue(xInputState_.Gamepad.sThumbLY, deadzone);
}

float Input::GetGamepadRightStickX() const {
	int deadzone = static_cast<int>(static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * deadzoneScale_);
	return NormalizeStickValue(xInputState_.Gamepad.sThumbRX, deadzone);
}

float Input::GetGamepadRightStickY() const {
	int deadzone = static_cast<int>(static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * deadzoneScale_);
	return NormalizeStickValue(xInputState_.Gamepad.sThumbRY, deadzone);
}

float Input::GetGamepadLeftTrigger() const
{
	return static_cast<float>(xInputState_.Gamepad.bLeftTrigger) / 255.0f;
}

float Input::GetGamepadRightTrigger() const
{
	return static_cast<float>(xInputState_.Gamepad.bRightTrigger) / 255.0f;
}

void Input::Vibrate(float left, float right, int durationMs)
{
	rumble_.active = true;
	rumble_.left = left;
	rumble_.right = right;
	rumble_.remainSec = static_cast<float>(durationMs) * 0.001f;
	ApplyVibration(left, right);
}

void Input::StopVibration()
{
	rumble_ = {};
	ApplyVibration(0.0f, 0.0f);
}
