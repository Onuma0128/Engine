#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <memory>
#include <vector>
#include <string>

#include "PrimitiveDrawrBase.h"
#include "PipelineStruct.h"

#include "ModelStruct.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;

/// <summary>
/// プリミティブの種類
/// </summary>
enum class PrimitiveType {
	None,
	Plane,
	Sphere,
	Ring,
	Cylinder,
	Skybox
};

/// <summary>
/// プリミティブ描画クラス
/// </summary>
class PrimitiveDrawr
{
public:

	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};

	/// <summary>
	/// テクスチャデータ
	/// </summary>
	struct TextureData {
		std::string directoryPath;
		std::string filePath;
		uint32_t textureIndex = 0;
	};

	/// <summary>
	/// マテリアルデータ
	/// </summary>
	struct MaterialData {
		Vector4 color;
		Matrix4x4 uvTransform;
		int32_t xTexcoord_alpha;
		int32_t yTexcoord_alpha;
		float padding[2];
	};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PrimitiveDrawr();

	/* =============== トレイルエフェクト =============== */

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></4頂点の座標を入れる>
	void Init(std::vector<Vector3> pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/* =============== Typeを選べるように =============== */

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="type"></PrimitiveTypeを決める>
	/// <param name="kIndex"></param>
	void TypeInit(PrimitiveType type, uint32_t kIndex = 0);

	/// <summary>
	/// シーンレンダラーを設定
	/// </summary>
	void SetSceneRenderer();

	/// <summary>
	/// シーンレンダラーを削除
	/// </summary>
	void RemoveRenderer();

	/// <summary>
	/// 描画
	/// </summary>
	void TypeDraw();

	/* =============== アクセッサ(トレイル用) =============== */

	/// <summary>
	/// 頂点の座標を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(std::vector<Vector3> pos);

	/* =============== アクセッサ(全体) =============== */

	Transform3D& GetTransform() { return transform_; }
	Transform2D& GetUVTransform() { return uvTransform_; }
	void SetBlendMode(BlendMode blendMode) { blendMode_ = blendMode; }
	const Vector4& GetColor() { return materialData_->color; }
	PrimitiveType GetPrimitiveType()const { return type_; }
	bool GetIsBillboard()const { return isBillboard_; }
	RenderOptions& GetRenderOptions() { return renderOptions_; }

	void SetTransform(Transform3D transform) { transform_ = transform; }
	void SetUVTransform(Transform2D transform) { uvTransform_ = transform; }
	void SetTexture(const std::string& filePath);
	void SetColor(const Vector3& color) {
		materialData_->color.x = color.x;
		materialData_->color.y = color.y;
		materialData_->color.z = color.z;
	}
	void SetAlpha(const float alpha) { materialData_->color.w = alpha; }
	void SetTexcoordX_Alpha(bool flag) { materialData_->xTexcoord_alpha = flag; }
	void SetTexcoordY_Alpha(bool flag) { materialData_->yTexcoord_alpha = flag; }
	void SetIsBillboard(bool flag) { isBillboard_ = flag; }

private:

	void CreateBufferResource(ComPtr<ID3D12Resource>& resource, size_t size);

	void CreateVertexBufferView(uint32_t kVertexSize);
	void CreateIndexBufferView();

	void CreateVertexData();
	void CreateIndexData();
	void CreateMaterialData();
	void CreateWVPData();

	void UVTransformUpdate();

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

	/* =============== Cylinder =============== */

	void InitCylinder(uint32_t kCylinderDivide);

	void DrawCylinder();

	// Cylinderの頂点計算
	void CreateCylinderVertexData(VertexData* vertexData, uint32_t kCylinderDivide);

	/* =============== Skybox =============== */

	void InitSkybox();

	void DrawSkybox();

	// Skyboxの頂点計算
	void CreateSkyboxVertexData(VertexData* vertexData);

private:

	std::unique_ptr<PrimitiveDrawrBase> primitiveDrawrBase_ = nullptr;

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

	PrimitiveType type_ = PrimitiveType::None;

	Transform3D transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform2D uvTransform_ = { {1.0f,1.0f},{0.0f},{0.0f,0.0f} };

	BlendMode blendMode_ = BlendMode::kBlendModeNormal;
	bool isBillboard_ = false;

	// 描画するか
	RenderOptions renderOptions_;

	/* =============== Trail =============== */

	std::vector<Vector3> positions_;
	
	/* =============== Sphere =============== */

	uint32_t kSubdivision_;

	/* =============== Ring =============== */

	uint32_t kRingDivide_;

	/* =============== Cylinder =============== */

	uint32_t kCylinderDivide_;

	/* =============== Skybox =============== */



};