#include "Audio.h"

#include <cassert>
#include <algorithm>

#include "AudioManager.h"

Audio::Audio()
{
    HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    assert(SUCCEEDED(hr));

    hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
    assert(SUCCEEDED(hr));
}

Audio::~Audio()
{
    if (masterVoice_) {
        masterVoice_->DestroyVoice();
        masterVoice_ = nullptr;
    }
    xAudio2_.Reset();
}

void Audio::SoundPlayWave(const std::string& filePath, float volume, bool loop)
{
    const AudioManager::SoundData& soundData = AudioManager::GetInstance()->GetSoundData(filePath);

    IXAudio2SourceVoice* sourceVoice = nullptr;
    HRESULT hr = xAudio2_->CreateSourceVoice(&sourceVoice, &soundData.wfex);
    assert(SUCCEEDED(hr) && sourceVoice);

    XAUDIO2_BUFFER buffer = {};
    buffer.pAudioData = soundData.pBuffer;
    buffer.AudioBytes = soundData.bufferSize;
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    if (loop) {
        buffer.LoopBegin = 0;
        buffer.LoopLength = 0;
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
        buffer.Flags = 0;
    }

    hr = sourceVoice->SubmitSourceBuffer(&buffer);
    assert(SUCCEEDED(hr));

    // 再生開始前に音量を設定
    hr = sourceVoice->SetVolume(volume);
    assert(SUCCEEDED(hr));

    hr = sourceVoice->Start();
    assert(SUCCEEDED(hr));

    // 管理配列に登録
    playingVoices_[filePath].push_back(sourceVoice);
}

void Audio::StopAudio(const std::string& filePath)
{
    auto it = playingVoices_.find(filePath);
    if (it == playingVoices_.end()) return;

    for (IXAudio2SourceVoice* v : it->second) {
        if (!v) continue;
        v->Stop(0);
        v->FlushSourceBuffers();
        v->DestroyVoice();
    }
    it->second.clear();
    playingVoices_.erase(it);
}

void Audio::SetMasterVolume(float volume)
{
    if (masterVoice_) {
        masterVoice_->SetVolume(volume);
    }
}

void Audio::SetVolume(const std::string& filePath, float volume)
{
    auto it = playingVoices_.find(filePath);
    if (it == playingVoices_.end()) return;

    for (IXAudio2SourceVoice* v : it->second) {
        if (!v) continue;
        v->SetVolume(volume);
    }
}

void Audio::StopAll()
{
    for (auto& kv : playingVoices_) {
        for (IXAudio2SourceVoice* v : kv.second) {
            if (!v) continue;
            v->Stop(0);
            v->FlushSourceBuffers();
            v->DestroyVoice();
        }
    }
    playingVoices_.clear();
}
