#pragma once

#include <xaudio2.h>
#include <wrl.h>

#include <string>
#include <unordered_map>
#include <vector>

using Microsoft::WRL::ComPtr;

/// <summary>
/// オーディオクラス
/// </summary>
class Audio
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Audio();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Audio();

	/// <summary>
	/// WAVファイルを再生する
	/// </summary>
	/// <param name="filePath"></オーディオファイル>
	/// <param name="volume"></音量>
	/// <param name="loop"></ループするか>
	void SoundPlayWave(const std::string& filePath, float volume = 1.0f, bool loop = false);

	/// <summary>
	/// オーディオファイルを停止する
	/// </summary>
	/// <param name="filePath"></停止するオーディオファイル>
	void StopAudio(const std::string& filePath);

	/// <summary>
	/// マスターボリュームを設定する
	/// </summary>
	/// <param name="volume"></音量>
	void SetMasterVolume(float volume);

	/// <summary>
	/// オーディオファイルの音量を設定する
	/// </summary>
	/// <param name="filePath"></設定するオーディオファイル>
	/// <param name="volume"></音量>
	void SetVolume(const std::string& filePath, float volume);

	/// <summary>
	/// 指定したオーディオが再生中か（同一ファイルのボイスが1つでも生きていればtrue）
	/// </summary>
	bool IsPlaying(const std::string& filePath);

private:

	/// <summary>
	/// 全てのオーディオファイルを停止する
	/// </summary>
	void StopAll();

private:
	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	std::unordered_map<std::string, std::vector<IXAudio2SourceVoice*>> playingVoices_;

};