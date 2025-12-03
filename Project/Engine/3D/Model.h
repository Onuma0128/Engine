#pragma once

#include "assimp//Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "ModelStruct.h"
#include "AnimationStruct.h"

/// <summary>
/// モデルクラス
/// </summary>
class Model
{
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void Initialize(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// バッファをバインド
	/// </summary>
	/// <param name="isAnimation"></param>
	void BindBuffers(bool isAnimation) const;

	/// <summary>
	/// マテリアルをバインド
	/// </summary>
	/// <param name="meshIdx"></param>
	void BindMaterial(uint32_t meshIdx) const;


	/// <summary>
	/// アクセッサ
	/// </summary>
	/// <returns></returns>
	
	// モデルベース取得
	const ModelData& GetModelData()const { return modelData_; }
	// ルートノードのローカル行列取得
	const Matrix4x4& GetLocalMatrix()const { return modelData_.rootNode.localMatrix; }
	// メッシュデータ取得
	const std::vector<MeshData>& GetMeshData()const { return modelData_.meshs; }
	// 頂点バッファビュー取得
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer() { return vertexBufferView_; }
	// インデックスバッファビュー取得
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	// テクスチャ設定
	void SetTexture(const std::string& directoryPath, const std::string& filename);
	// 環境マップテクスチャ設定
	void SetTexture_ENV(const std::string& directoryPath, const std::string& filename);

private:

	/// <summary>
	/// 頂点データを作成
	/// </summary>
	void MakeVertexData();

	/// <summary>
	/// インデックスデータを作成
	/// </summary>
	void MakeIndexData();

	/// <summary>
	/// メッシュごとのカラーを作成
	/// </summary>
	/// <param name="material"></param>
	void MakeMeshColor(MaterialData& material);

	// 文字列をワイド文字列に変換
	static std::wstring s2ws(const std::string& str);
	// マテリアルテンプレートファイルを読み込む
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// ノードを再帰的に読み込む
	static Node ReadNode(aiNode* node);


private:

	/*==================== モデルのデータ ====================*/

	ModelData modelData_;

	std::vector<KdColor*> kdColors_{};

	/*==================== 頂点データ ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	/*==================== Indexデータ ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	uint32_t* indexData_ = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

};