#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <unordered_map>

#define NOMINMAX

#include "Model.h"

#include "ModelStruct.h"

// パーティクルのMAX値(上げすぎ注意!!)
const uint32_t kNumMaxInstance = 1024;

class ParticleEmitter;
class ParticleEditor;
namespace NumaEngine { class DirectXEngine; }
class SrvManager;

/// <summary>
/// パーティクルを管理するクラス
/// </summary>
class ParticleManager
{
public:
	
    /// <summary>
    /// 頂点データ
	/// </summary>
    struct VertexData {
		NumaEngine::Vector4 position;
		NumaEngine::Vector2 texcoord;
		NumaEngine::Vector3 normal;
	};

    /// <summary>
    /// マテリアルデータ
    /// </summary>
    struct Material {
		NumaEngine::Vector4 color;
		int32_t enableLighting;
		float padding[3];
      NumaEngine::Matrix4x4 uvTransform;
		float shininess;
		int32_t isUVFlipX;
		int32_t isUVFlipY;
		int32_t _pad1;
	};

    /// <summary>
    /// パーティクルデータ
    /// </summary>
    struct Particle {
		EulerTransform transform;
		NumaEngine::Vector3 offsetScale;
		NumaEngine::Vector3 uvTranslate;
		NumaEngine::Vector3 velocity;
		NumaEngine::Vector3 rotateSpeed;
		NumaEngine::Vector4 color;
		float lifeTime;
		float currentTime;
		uint16_t emitterID;
	};

    /// <summary>
    /// GPU用パーティクルデータ構造体
    /// </summary>
    struct ParticleForGPU {
      NumaEngine::Matrix4x4 WVP;
		NumaEngine::Matrix4x4 World;
		NumaEngine::Vector4 color;
	};

	/// <summary>
	/// パーティクルグループデータ
	/// </summary>
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

		uint16_t nextEmitterId = 0;
	};

private:

	// シングルトンインスタンス
	static std::unique_ptr<ParticleManager> instance_;

public:

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;

	// シングルトンインスタンスの取得
	static ParticleManager* GetInstance();

    // 初期化
	void Initialize(NumaEngine::DirectXEngine* dxEngine);

	// 更新
	void Update();

	// 描画
	void Draw();

	// グループのクリア処理
	void Clear();

	// 終了処理
	void Finalize();

	// パーティクルグループの作成
	void CreateParticleGroup(std::shared_ptr<ParticleEmitter> emitter);

	// パーティクルの発生
	void Emit(const std::string name);

private:

	// エディタ用更新
	void ParticleEditorUpdate();

	// 頂点関連の生成
	void CreateVertexData();
	void CreateVertexResource();

	// インデックス関連の生成
	void CreateIndexData();
	void CreateIndexResource();

	// マテリアルリソースの生成
	void CreateMatrialResource(ParticleGroup& group);

	// インスタンスリソースの生成
	void EnsureInstanceCapacity(ParticleGroup& group, uint32_t required);

private:

	/*==================== メンバ変数 ====================*/

    // 基盤ポインタ
	NumaEngine::DirectXEngine* dxEngine_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// パイプラインステート
	std::array<ComPtr<ID3D12PipelineState>, 5> pipelineStates_;

	/* =============== 頂点 =============== */

	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexDatas {
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		ComPtr<ID3D12Resource> vertexResource = nullptr;
		VertexData* vertexData = nullptr;
	};
	VertexDatas vertexPlane_;
	VertexDatas vertexRing_;

	/* =============== index頂点 =============== */

	/// <summary>
	/// インデックスデータ構造体
	/// </summary>
	struct IndexDatas {
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};
		ComPtr<ID3D12Resource> indexResource = nullptr;
		uint32_t* indexData = nullptr;
	};
	IndexDatas indexPlane_;
	IndexDatas indexRing_;

	// パーティクルグループコンテナ
	std::unordered_map<std::string, ParticleGroup> particleGroups_;
	// ラインの描画を決める
	bool drawEmitter_ = false;


};

