#include "Model.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "DirectXEngine.h"
#include "ModelBase.h"
#include "TextureManager.h"
#include "SrvManager.h"
#include "CreateBufferResource.h"


void Model::Initialize(const std::string& directoryPath, const std::string& filename)
{
    this->modelBase_ = ModelBase::GetInstance();

    modelData_ = LoadObjFile(directoryPath, filename);

    for (auto& material : modelData_.materials) {

        TextureManager::GetInstance()->LoadTexture(material.directoryPath, material.filePath);

        material.textureIndex =
            TextureManager::GetInstance()->GetSrvIndex(material.filePath);

        material.ENV_DirectoryPath = "resources";
        material.ENV_FilePath = "output.dds";
        TextureManager::GetInstance()->LoadTexture(material.ENV_DirectoryPath, material.ENV_FilePath);

        material.ENV_TextureIndex =
            TextureManager::GetInstance()->GetSrvIndex(material.ENV_FilePath);

        MakeMeshColor(material);
    }

    MakeVertexData();
    MakeIndexData();
}

void Model::BindBuffers(bool isAnimation) const
{
    auto commandList = modelBase_->GetDxEngine()->GetCommandList();
    if (!isAnimation) {
        commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    }
    commandList->IASetIndexBuffer(&indexBufferView_);
}

void Model::BindMaterial(uint32_t meshIdx) const
{
    auto commandList = modelBase_->GetDxEngine()->GetCommandList();
    const auto& material = modelData_.materials[meshIdx];
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, material.textureIndex);
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(7, material.ENV_TextureIndex);
    commandList->SetGraphicsRootConstantBufferView(8, material.kdColorResource->GetGPUVirtualAddress());
}

void Model::MakeVertexData()
{
    // 実際に頂点リソースを作る
    vertexResource_ = CreateBufferResource(modelBase_->GetDxEngine()->GetDevice(), sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

void Model::MakeIndexData()
{
    // 実際に頂点リソースを作る
    indexResource_ = CreateBufferResource(modelBase_->GetDxEngine()->GetDevice(), sizeof(uint32_t) * modelData_.indices.size());
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
    std::memcpy(indexData_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
}

void Model::MakeMeshColor(MaterialData& material)
{
    // マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
    material.kdColorResource = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(KdColor));
    // 書き込むためのアドレスを取得
    KdColor* kdColor = nullptr;
    material.kdColorResource->Map(0, nullptr, reinterpret_cast<void**>(&kdColor));
    kdColor->Color = material.kdColor;
    kdColors_.push_back(std::move(kdColor));
}

std::wstring Model::s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + filename;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_RemoveRedundantMaterials);
    assert(scene->HasMeshes());

    ModelData modelData;
    size_t vertexOffset = 0;
    // ファイルを保存
    modelData.directoryPath = directoryPath;
    modelData.filePath = filename;

    // meshを解析
    for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        aiMesh* mesh = scene->mMeshes[meshIndex];
        assert(mesh->HasNormals());

        MeshData meshData{};
        meshData.indexStart = static_cast<uint32_t>(modelData.indices.size());
        meshData.materialIndex = mesh->mMaterialIndex;
        uint32_t baseVertexOffset = static_cast<uint32_t>(modelData.vertices.size());
        //modelData.vertices.resize(mesh->mNumVertices);

        for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
            VertexData vertex{};
            aiVector3D& position = mesh->mVertices[vertexIndex];
            aiVector3D& normal = mesh->mNormals[vertexIndex];

            vertex.position = { -position.x,position.y,position.z,1.0f };
            vertex.normal = { -normal.x,normal.y,normal.z };

            if (mesh->HasTextureCoords(0)) {
                aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
                vertex.texcoord = { texcoord.x,texcoord.y };
            }
            modelData.vertices.push_back(vertex);
        }

        for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
            aiFace& face = mesh->mFaces[faceIndex];
            assert(face.mNumIndices == 3);

            for (uint32_t element = 0; element < face.mNumIndices; ++element) {
                uint32_t vertexIndex = face.mIndices[element];
                modelData.indices.push_back(static_cast<uint32_t>(baseVertexOffset) + vertexIndex);
            }
        }
        meshData.indexCount = static_cast<uint32_t>(modelData.indices.size()) - meshData.indexStart;
        modelData.meshs.push_back(meshData);
        vertexOffset = modelData.vertices.size();

        for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
            aiBone* bone = mesh->mBones[boneIndex];
            std::string jointName = bone->mName.C_Str();
            JointWeightData& jointWeightData = modelData.skinClusterData[jointName];

            aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
            aiVector3D scale, translate;
            aiQuaternion rotate;
            bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
            Matrix4x4 bindPoseMatrix = Matrix4x4::Affine(
                Vector3{ scale.x,scale.y,scale.z },
                Quaternion{ rotate.x,-rotate.y,-rotate.z,rotate.w },
                Vector3{ -translate.x,translate.y,translate.z }
            );
            jointWeightData.inverseBindPosMatrix = Matrix4x4::Inverse(bindPoseMatrix);
            
            for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
                const auto& w = bone->mWeights[weightIndex];

                uint32_t globalIndex = static_cast<uint32_t>(baseVertexOffset) + w.mVertexId;
                jointWeightData.vertexWeights.push_back({ w.mWeight,globalIndex });
            }
        }
    }

    // materialを解析
    modelData.materials.resize(scene->mNumMaterials);
    for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
        aiMaterial* material = scene->mMaterials[materialIndex];
        MaterialData materialData{};
        aiString textureFilePath;
        aiColor3D kd(1, 1, 1);
        Vector4 kdColor = { 1.0f,1.0f,1.0f,1.0f };

        // Diffuseテクスチャを確認
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath) == AI_SUCCESS) {
            materialData.directoryPath = directoryPath;
            materialData.filePath = textureFilePath.C_Str();

        // いずれのテクスチャも見つからなければデフォルトのwhite1x1.pngを割り当てる
        } else {
            materialData.directoryPath = "resources";
            materialData.filePath = "white1x1.png";
            if (material->Get(AI_MATKEY_COLOR_DIFFUSE, kd) == AI_SUCCESS) {
                kdColor = { kd.r, kd.g, kd.b, 1.0f };
            }
        }

        materialData.kdColor = kdColor;
        modelData.materials[materialIndex] = materialData;
    }

    modelData.rootNode = ReadNode(scene->mRootNode);

    // アニメーションがあればアニメーションを読む
    if (scene->mNumAnimations > 0) {

        for (uint32_t animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex) {

            aiAnimation* animationAssimp = scene->mAnimations[animationIndex];
            AnimationData animationData;
            animationData.name = animationAssimp->mName.C_Str();
            animationData.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

            for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
                aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
                NodeAnimation& nodeAnimation = animationData.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

                // 座標
                for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
                    aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
                    KeyFrameVector3 keyframe;
                    keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
                    keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
                    nodeAnimation.translate.keyframes.push_back(keyframe);
                }
                // 回転
                for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
                    aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
                    KeyFrameQuaternion keyframe;
                    keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
                    keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };
                    nodeAnimation.rotate.keyframes.push_back(keyframe);
                }
                // スケール
                for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
                    aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
                    KeyFrameVector3 keyframe;
                    keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
                    keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };
                    nodeAnimation.scale.keyframes.push_back(keyframe);
                }
            }
            modelData.animations.push_back(std::move(animationData));
        }
    }

    return modelData;
}

void Model::SetTexture(const std::string& directoryPath, const std::string& filename)
{
    if (modelData_.materials.empty()) { return; }
    auto& materialData = modelData_.materials[0];
    materialData.directoryPath = directoryPath + "/";
    materialData.filePath = filename;

    TextureManager::GetInstance()->LoadTexture(materialData.directoryPath, materialData.filePath);
    materialData.textureIndex =
        TextureManager::GetInstance()->GetSrvIndex(materialData.filePath);
}

void Model::SetTexture_ENV(const std::string& directoryPath, const std::string& filename)
{
    if (modelData_.materials.empty()) { return; }
    auto& materialData = modelData_.materials[0];
    materialData.ENV_DirectoryPath = directoryPath + "/";
    materialData.ENV_FilePath = filename;

    TextureManager::GetInstance()->LoadTexture(materialData.ENV_DirectoryPath, materialData.ENV_FilePath);
    materialData.ENV_TextureIndex =
        TextureManager::GetInstance()->GetSrvIndex(materialData.ENV_FilePath);
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
    MaterialData materialData; // 構築するMaterialData
    std::string line; // ファイルから読んだ1行を格納するもの
    std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
    assert(file.is_open()); // とりあえず開けなかったら止める
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream s(line);
        s >> identifier;
        // identifierに応じた処理
        if (identifier == "map_Kd") {
            std::string textureFilename;
            s >> textureFilename;
            //連結してファイルパスにする
            materialData.directoryPath = directoryPath + "/";
            materialData.filePath = textureFilename;
        }
    }
    std::string materialTexture = materialData.directoryPath + materialData.filePath;
    if (materialTexture.empty()) {
        std::string textureFilename = "white1x1.png";
        materialData.directoryPath = directoryPath + "/";
        materialData.filePath = textureFilename;
    }
    return materialData;
}

Node Model::ReadNode(aiNode* node)
{
    Node result;
    
    aiVector3D scale, translate;
    aiQuaternion rotate;
    node->mTransformation.Decompose(scale, rotate, translate);
    result.transform.scale = { scale.x,scale.y,scale.z };
    result.transform.rotation = { rotate.x,-rotate.y,-rotate.z,rotate.w };
    result.transform.translation = { -translate.x,translate.y,translate.z };
    result.localMatrix = Matrix4x4::Affine(result.transform.scale, result.transform.rotation, result.transform.translation);
    result.globalMatrix = result.localMatrix;
    result.name = node->mName.C_Str();
    result.children.resize(node->mNumChildren);
    for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
        result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
    }
    return result;
}