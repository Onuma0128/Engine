#pragma once
#include <map>
#include <string>
#include <memory>

class DirectXEngine;
class Model;

/// <summary>
/// モデルを管理するクラス
/// </summary>
class ModelManager
{
private:

	// シングルトンインスタンス
	static std::unique_ptr<ModelManager> instance_;

public:

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;

	// シングルトンインスタンスの取得
	static ModelManager* GetInstance();

	// 終了処理
	void Finalize();

	// モデルのロード
	void LoadModel(const std::string& directoryPath, const std::string& filePath);

	// モデルの検索
	Model* FindModel(const std::string& filePath);

private:

	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models_;
};