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


struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct MeshData {
	uint32_t indexStart;
	uint32_t indexCount;
	uint32_t materialIndex;
};

struct Material {
	Vector4 color;
	Matrix4x4 uvTransform;
	int32_t enableDraw;
	int32_t enableLighting;
	float shininess;
	float environmentCoefficient;
};

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

struct KdColor {
	Vector4 Color;
};

struct Node {
	Transform3D transform;
	Matrix4x4 localMatrix;
	Matrix4x4 globalMatrix;
	std::string name;
	std::vector<Node> children;
};

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPosMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

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

static const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<uint32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;
};

struct SkinCluster {
	// Indexを保存
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	// Influence
	ComPtr<ID3D12Resource> infuenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	// MatrixPalette
	std::vector<WellForGPU> mappedPalettes;
};

struct RenderOptions {
	bool enabled;		// 描画をするか
	bool offscreen;		// オフスク描画をするか
};