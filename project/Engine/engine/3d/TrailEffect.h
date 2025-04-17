#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <vector>
#include <string>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;

class TrailEffectBase;

enum class PrimitiveType {
	Plane,
	Sphere,
	Ring
};

class TrailEffect
{
public:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};
	struct TextureData {
		std::string directoryPath;
		std::string filePath;
		uint32_t textureIndex = 0;
	};
	struct MaterialData {
		Vector4 color;
		int32_t xTexcoord_alpha;
		int32_t yTexcoord_alpha;
		float padding[2];
	};

	/* =============== トレイルエフェクト =============== */

	void Init(std::vector<Vector3> pos);

	void Update();

	void Draw();

	/* =============== Typeを選べるように =============== */

	void TypeInit(PrimitiveType type, uint32_t kIndex = 0);

	void TypeDraw();

	/* =============== アクセッサ(トレイル用) =============== */

	void SetPosition(std::vector<Vector3> pos);

	/* =============== アクセッサ(全体) =============== */

	EulerTransform& GetTransform() { return transform_; }
	const Vector4& GetColor() { return materialData_->color; }


	void SetTransform(EulerTransform transform) { transform_ = transform; }
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	void SetColor(const Vector3& color) {
		materialData_->color.x = color.x;
		materialData_->color.y = color.y;
		materialData_->color.z = color.z;
	}
	void SetAlpha(const float alpha) { materialData_->color.w = alpha; }
	void SetTexcoordX_Alpha(bool flag) { materialData_->xTexcoord_alpha = flag; }
	void SetTexcoordY_Alpha(bool flag) { materialData_->yTexcoord_alpha = flag; }

private:

	void CreateBufferResource(ComPtr<ID3D12Resource>& resource, size_t size);

	void CreateVertexBufferView(uint32_t kVertexSize);
	void CreateIndexBufferView();

	void CreateVertexData();
	void CreateIndexData();
	void CreateMaterialData();
	void CreateWVPData();

	/* =============== Plane =============== */

	void InitPlane();

	void DrawPlane();

	// Planeの頂点計算
	void CreatePlaneVertexData(VertexData* vertexData);


	/* =============== Sphere =============== */

	void InitSphere(uint32_t kSubdivision);

	void DrawSphere();

	// Sphereの頂点計算
	void CreateSphereVertexData(VertexData* vertexData, uint32_t kSubdivision);

	/* =============== Ring =============== */

	void InitRing(uint32_t kRingDivide);

	void DrawRing();

	// Ringの頂点計算
	void CreateRingVertexData(VertexData* vertexData, uint32_t kRingDivide);

private:

	TrailEffectBase* trailEffectBase_ = nullptr;

	/* =============== 頂点 =============== */

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	VertexData* vertexData_ = nullptr;

	/* =============== index頂点 =============== */

	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	uint32_t* indexData_ = nullptr;

	/* =============== マテリアル =============== */

	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	MaterialData* materialData_ = nullptr;
	// TextureData
	TextureData textureData_;

	/* =============== 座標変換行列 =============== */

	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Matrix4x4* wvpData_ = nullptr;

	/* =============== TypeとTransform =============== */

	PrimitiveType type_;

	EulerTransform transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	/* =============== Trail =============== */

	std::vector<Vector3> positions_;
	
	/* =============== Sphere =============== */

	uint32_t kSubdivision_;

	/* =============== Ring =============== */

	uint32_t kRingDivide_;

};