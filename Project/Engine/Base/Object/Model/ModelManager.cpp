#include "ModelManager.h"

#include "DirectXEngine.h"
#include "Model.h"

std::unique_ptr<ModelManager> ModelManager::instance_ = nullptr;

ModelManager* ModelManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = std::make_unique<ModelManager>();
    }
    return instance_.get();
}

void ModelManager::Finalize()
{
    instance_ = nullptr;
}

void ModelManager::LoadModel(const std::string& directoryPath, const std::string& filePath)
{
    if (models_.contains(filePath)) {
        // 読み込み済みなら早期return
        return;
    }
    // モデルの生成とファイル読み込み、初期化
    std::unique_ptr<Model> model = std::make_unique<Model>();
    model->Initialize(directoryPath, filePath);

    // モデルをmapコンテナに格納する
    models_.insert(std::make_pair(filePath, std::move(model)));
}

Model* ModelManager::FindModel(const std::string& filePath)
{
    if (models_.contains(filePath)) {
        // 読み込みモデルを戻り値としてreturn
        return models_.at(filePath).get();
    }
    return nullptr;
}
