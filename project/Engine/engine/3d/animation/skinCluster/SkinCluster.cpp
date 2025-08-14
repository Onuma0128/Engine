#include "SkinCluster.h"

#include <cassert>

#include "DirectXEngine.h"
#include "CreateBufferResource.h"

#include "animation/skeleton/Skeleton.h"

void SkinCluster::CreateSkinCluster(Skeleton& skeleton, const ModelData& modelData)
{
	// **Influenceリソースの作成**
	size_t influenceSize = modelData.vertices.size() * sizeof(VertexInfluence);
	item_.infuenceResource = CreateBufferResource(DirectXEngine::GetDevice(), influenceSize);

	// **Influenceのマッピング**
	VertexInfluence* mappedInfluence = nullptr;
	item_.infuenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, influenceSize);
	item_.mappedInfluence = std::span<VertexInfluence>(mappedInfluence, modelData.vertices.size());

	// **Influence バッファビューの設定**
	item_.influenceBufferView.BufferLocation = item_.infuenceResource->GetGPUVirtualAddress();
	item_.influenceBufferView.SizeInBytes = static_cast<UINT>(influenceSize);
	item_.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	item_.mappedPalettes.resize(skeleton.GetJoints().size());
	for (size_t i = 0; i < item_.mappedPalettes.size(); ++i) {
		item_.mappedPalettes[i].skeletonSpaceMatrix = Matrix4x4::Identity();
		item_.mappedPalettes[i].skeletonSpaceInverseTransposeMatrix = Matrix4x4::Identity();
	}
	item_.inverseBindPoseMatrices.resize(skeleton.GetJoints().size());
	for (size_t i = 0; i < item_.inverseBindPoseMatrices.size(); ++i) {
		item_.inverseBindPoseMatrices[i] = Matrix4x4::Identity();
	}

	for (const auto& jointWeight : modelData.skinClusterData) {
		// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		auto it = skeleton.GetItem().jointMap.find(jointWeight.first);
		if (skeleton.CheckJointName(jointWeight.first)) { // そんな名前のjointは存在しない。なので次に回す
			continue;
		}
		// (*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		item_.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPosMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = item_.mappedInfluence[vertexWeight.vertexIndex];
			// 空いている場所に入れる
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				// weight == 0.0fが開いている状態なので、その場所にweightとjointのindexを代入
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}
}

void SkinCluster::SkinClusterUpdate(Skeleton& skeleton)
{
	for (size_t jointIndex = 0; jointIndex < skeleton.GetJoints().size(); ++jointIndex) {
		assert(jointIndex < item_.inverseBindPoseMatrices.size());
		item_.mappedPalettes[jointIndex].skeletonSpaceMatrix =
			item_.inverseBindPoseMatrices[jointIndex] * skeleton.GetJoint(jointIndex).GetSkeletonSpaceMatrix();
		item_.mappedPalettes[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Matrix4x4::Inverse(item_.mappedPalettes[jointIndex].skeletonSpaceMatrix).Transpose();
	}
}
