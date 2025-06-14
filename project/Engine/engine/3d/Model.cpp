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
    }

    MakeVertexData();
    MakeIndexData();
}

void Model::Draw(bool isAnimation)
{
    auto commandList = modelBase_->GetDxEngine()->GetCommandList();
    if (!isAnimation) {
        commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    }
    commandList->IASetIndexBuffer(&indexBufferView_);

    for (const auto& subMesh : modelData_.subMeshes) {
        const auto& material = modelData_.materials[subMesh.materialIndex];
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, material.textureIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(7, material.ENV_TextureIndex);
        // 描画
        commandList->DrawIndexedInstanced(subMesh.indexCount, 1, subMesh.indexStart, 0, 0);
    }
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
    const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
    assert(scene->HasMeshes());

    ModelData modelData;
    size_t vertexOffset = 0;

    // meshを解析
    for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        aiMesh* mesh = scene->mMeshes[meshIndex];
        assert(mesh->HasNormals());

        SubMesh subMesh{};
        subMesh.indexStart = static_cast<uint32_t>(modelData.indices.size());
        subMesh.materialIndex = mesh->mMaterialIndex;
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
                modelData.indices.push_back(static_cast<uint32_t>(vertexOffset) + vertexIndex);
            }
        }
        subMesh.indexCount = static_cast<uint32_t>(modelData.indices.size()) - subMesh.indexStart;
        modelData.subMeshes.push_back(subMesh);
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
                jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
            }
        }
    }

    // materialを解析
    modelData.materials.resize(scene->mNumMaterials);
    for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
        aiMaterial* material = scene->mMaterials[materialIndex];
        MaterialData materialData{};
        aiString textureFilePath;

        // Diffuseテクスチャを確認
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath) == AI_SUCCESS) {
            materialData.directoryPath = directoryPath;
            materialData.filePath = textureFilePath.C_Str();

        // いずれのテクスチャも見つからなければデフォルトのwhite1x1.pngを割り当てる
        } else {
            materialData.directoryPath = "resources";
            materialData.filePath = "white1x1.png";
        }

        modelData.materials[materialIndex] = materialData;
    }

    modelData.rootNode = ReadNode(scene->mRootNode);

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

    result.name = node->mName.C_Str();
    result.children.resize(node->mNumChildren);
    for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
        result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
    }
    return result;
}