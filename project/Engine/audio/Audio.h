#pragma once

#include <xaudio2.h>
#include <wrl.h>

#include <string>
#include <unordered_map>
#include <vector>

using Microsoft::WRL::ComPtr;

class Audio
{
public:
	Audio();
	~Audio();

	void SoundPlayWave(const std::string& filePath, float volume = 1.0f, bool loop = false);

	void StopAudio(const std::string& filePath);

	void SetMasterVolume(float volume);

	void SetVolume(const std::string& filePath, float volume);

private:

	void StopAll();

private:
	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	std::unordered_map<std::string, std::vector<IXAudio2SourceVoice*>> playingVoices_;

};