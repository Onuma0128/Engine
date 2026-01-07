#include "AudioManager.h"

#include <cassert>

std::unique_ptr<AudioManager> AudioManager::instance_ = nullptr;

AudioManager* AudioManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = std::make_unique<AudioManager>();
    }
    return instance_.get();
}

void AudioManager::Finalize()
{
    for (auto& pair : soundDataMap_) {
        SoundUnload(&pair.second);
    }
}

void AudioManager::LoadAudioFile(const std::string& directoryPath, const std::string& filePath)
{
	const std::string fullFilePath = directoryPath + "/" + filePath;

	if (soundDataMap_.find(filePath) == soundDataMap_.end()) {
		soundDataMap_[filePath] = SoundLoadWave(fullFilePath.c_str());
	}
}

const AudioManager::SoundData& AudioManager::GetSoundData(const std::string& filePath)
{
    auto it = soundDataMap_.find(filePath);
    if (it == soundDataMap_.end()) {
		// 未ロードならロードする
		assert(0);
    }
    return it->second;
}

AudioManager::SoundData AudioManager::SoundLoadWave(const char* filePath)
{
    std::ifstream file(filePath, std::ios_base::binary);
    assert(file.is_open());

    RiffHeader riff{};
    file.read(reinterpret_cast<char*>(&riff), sizeof(riff));
    assert(strncmp(riff.chunk.id, "RIFF", 4) == 0);
    assert(strncmp(riff.type, "WAVE", 4) == 0);

    FormatChunk format{};
    file.read(reinterpret_cast<char*>(&format.chunk), sizeof(ChunkHeader));
    assert(strncmp(format.chunk.id, "fmt ", 4) == 0);
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read(reinterpret_cast<char*>(&format.fmt), format.chunk.size);

    ChunkHeader data{};
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
    if (strncmp(data.id, "JUNK", 4) == 0) {
        file.seekg(data.size, std::ios_base::cur);
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
    }

    while (strncmp(data.id, "data", 4) != 0) {
        file.seekg(data.size, std::ios_base::cur);
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        assert(!file.eof() && "Failed to find 'data' chunk in WAV file.");
    }

    SoundData soundData{};
    soundData.wfex = format.fmt;
    soundData.buffer.resize(data.size);
    file.read(reinterpret_cast<char*>(soundData.buffer.data()), data.size);
    soundData.bufferSize = data.size;

    return soundData;
}

void AudioManager::SoundUnload(SoundData* soundData)
{
	soundData->buffer.clear();
	soundData->buffer.shrink_to_fit();
	soundData->bufferSize = 0;
	soundData->wfex = {};
}
