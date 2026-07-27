#include "ModelManager.h"

#include "DirectXEngine.h"
#include "Model.h"

std::unique_ptr<NumaEngine::ModelManager> NumaEngine::ModelManager::instance_ = nullptr;

NumaEngine::ModelManager* NumaEngine::ModelManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = std::make_unique<NumaEngine::ModelManager>();
    }
    return instance_.get();
}

void NumaEngine::ModelManager::Finalize()
{
    instance_ = nullptr;
}

void NumaEngine::ModelManager::LoadModel(const std::string& directoryPath, const std::string& filePath)
{
    if (models_.contains(filePath)) {
        // 読み込み済みなら早期return
        return;
    }
    // モデルの生成とファイル読み込み、初期化
    std::unique_ptr<NumaEngine::Model> model = std::make_unique<NumaEngine::Model>();
    model->Initialize(directoryPath, filePath);

    // モデルをmapコンテナに格納する
    models_.insert(std::make_pair(filePath, std::move(model)));
}

NumaEngine::Model* NumaEngine::ModelManager::FindModel(const std::string& filePath)
{
    if (models_.contains(filePath)) {
        // 読み込みモデルを戻り値としてreturn
        return models_.at(filePath).get();
    }
    return nullptr;
}
