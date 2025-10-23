#pragma once

#include <vector>
#include <string>
#include <map>
#include <span>
#include <array>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "AnimationStruct.h"

#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
using Microsoft::WRL::ComPtr;

/// <summary>
/// 頂点データを表す構造体
/// </summary>
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

/// <summary>
/// メッシュデータを表す構造体
/// </summary>
struct MeshData {
	uint32_t indexStart;
	uint32_t indexCount;
	uint32_t materialIndex;
};

/// <summary>
/// マテリアルデータを表す構造体
/// </summary>
struct Material {
	Vector4 color;
	Matrix4x4 uvTransform;
	int32_t enableDraw;
	int32_t enableLighting;
	int32_t outlineMask;
	int32_t outlineSceneColor;
	Vector3 outlineColor;
	float shininess;
	float environmentCoefficient;
};

/// <summary>
/// メッシュIDを表す構造体
/// </summary>
struct MeshID {
	int32_t index;
};

/// <summary>
/// マテリアルデータを表す構造体
/// </summary>
struct MaterialData {
	std::string directoryPath;
	std::string filePath;
	uint32_t textureIndex = 0;

	std::string ENV_DirectoryPath;
	std::string ENV_FilePath;
	uint32_t ENV_TextureIndex = 0;

	ComPtr<ID3D12Resource> kdColorResource;
	Vector4 kdColor;
};

/// <summary>
/// キャンバス上の色を表す構造体
/// </summary>
struct KdColor {
	Vector4 Color;
};

/// <summary>
/// ノードを表す構造体
/// </summary>
struct Node {
	Transform3D transform;
	Matrix4x4 localMatrix;
	Matrix4x4 globalMatrix;
	std::string name;
	std::vector<Node> children;
};

/// <summary>
/// 頂点のウェイトデータを表す構造体
/// </summary>
struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

/// <summary>
/// ジョイントのウェイトデータを表す構造体
/// </summary>
struct JointWeightData {
	Matrix4x4 inverseBindPosMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

/// <summary>
/// モデルデータを表す構造体
/// </summary>
struct ModelData {
	std::string directoryPath;
	std::string filePath;
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<AnimationData> animations;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	std::vector<MeshData> meshs;
	std::vector<MaterialData> materials;
	Node rootNode;
};

/// <summary>
/// レンダリングオプションを表す構造体
/// </summary>
struct RenderOptions {
	bool enabled;		// 描画をするか
	bool offscreen;		// オフスク描画をするか
};