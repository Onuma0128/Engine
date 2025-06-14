#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <unordered_map>

#include "Model.h"

#include "ModelStruct.h"

// パーティクルのMAX値(上げすぎ注意!!)
const uint32_t kNumMaxInstance = 1024;

class ParticleEmitter;
class ParticleEditor;
class DirectXEngine;
class SrvManager;

class ParticleManager
{
public:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	struct Particle {
		EulerTransform transform;
		Vector3 offsetScale;
		Vector3 uvTranslate;
		Vector3 velocity;
		Vector3 rotateSpeed;
		Vector4 color;
		float lifeTime;
		float currentTime;
		std::string emitterName;
	};
	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	struct ParticleGroup {
		std::string textureFilePath;
		uint32_t srvIndex;
		uint32_t textureIndex;
		std::list<Particle> particles;
		uint32_t maxInstance;
		uint32_t instancingIndex;
		ComPtr<ID3D12Resource> instancingResource;
		uint32_t instanceCount;
		ComPtr<ID3D12Resource> materialResource_ = nullptr;
		Material* materialData_ = nullptr;

		ParticleForGPU* instancingData;
		std::vector<std::weak_ptr<ParticleEmitter>> emitters;
		std::unique_ptr<ParticleEditor> editor;
	};

private:
	static ParticleManager* instance_;

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;
public:
	// シングルトンインスタンスの取得
	static ParticleManager* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void Update();

	void Draw();

	// グループのクリア処理
	void Clear();

	void Finalize();

	void CreateParticleGroup(std::shared_ptr<ParticleEmitter> emitter);

	void Emit(const std::string name);

private:

	void ParticleEditorUpdate();

	void CreateVertexData();
	void CreateVertexResource();

	void CreateIndexData();
	void CreateIndexResource();

	void CreateMatrialResource(ParticleGroup& group);

private:

	/*==================== メンバ変数 ====================*/

	// 基盤ポインタ
	DirectXEngine* dxEngine_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// パイプラインステート
	std::array<ComPtr<ID3D12PipelineState>, 5> pipelineStates_;

	/* =============== 頂点 =============== */

	struct VertexDatas {
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		ComPtr<ID3D12Resource> vertexResource = nullptr;
		VertexData* vertexData = nullptr;
	};
	VertexDatas vertexPlane_;
	VertexDatas vertexRing_;

	/* =============== index頂点 =============== */

	struct IndexDatas {
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};
		ComPtr<ID3D12Resource> indexResource = nullptr;
		uint32_t* indexData = nullptr;
	};
	IndexDatas indexPlane_;
	IndexDatas indexRing_;

	// パーティクルグループコンテナ
	std::unordered_map<std::string, ParticleGroup> particleGroups_;


};