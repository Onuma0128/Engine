#pragma once

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>

#include <memory>
#include <string>
#include <unordered_map>

/// <summary>
/// オーディオ管理クラス
/// </summary>
class AudioManager
{
public:

	/// <summary>
	/// チャンクヘッダー
	/// </summary>
	struct ChunkHeader {
		char id[4];
		int32_t size;
	};

	/// <summary>
	/// RIFFヘッダー
	/// </summary>
	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};

	/// <summary>
	/// フォーマットチャンク
	/// </summary>
	struct FormatChunk {
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	/// <summary>
	/// 音声データ
	/// </summary>
	struct SoundData {
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファの先頭アドレス
		std::vector<BYTE> buffer;
		// バッファのサイズ
		unsigned int bufferSize;
	};

private:

	/// <summary>
	/// インスタンス
	/// </summary>
	static std::unique_ptr<AudioManager> instance_;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioManager() = default;

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name=""></param>
	AudioManager(AudioManager&) = delete;

	/// <summary>
	/// 代入演算子
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	AudioManager& operator=(AudioManager&) = delete;

	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns></returns>
	static AudioManager* GetInstance();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// オーディオファイルを読み込む
	/// </summary>
	/// <param name="directoryPath"></ファイルまでのディレクトリパス>
	/// <param name="filePath"></ファイルパス>
	void LoadAudioFile(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// オーディオファイルの音声データを取得する
	/// </summary>
	/// <param name="filePath"></音声データが欲しいファイルパス>
	/// <returns></returns>
	const SoundData& GetSoundData(const std::string& filePath);

private:

	/// <summary>
	/// WAVファイルを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	SoundData SoundLoadWave(const char* filePath);

	/// <summary>
	/// 音声データの解放
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnload(SoundData* soundData);

private:

	std::unordered_map<std::string, SoundData> soundDataMap_;
};