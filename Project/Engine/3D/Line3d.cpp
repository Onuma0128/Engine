#include "Line3d.h"

#include <numbers>

#include "LineInstanceRenderer.h"

#include "Camera.h"
#include "CameraManager.h"
#include "CreateBufferResource.h"

void Line3d::Initialize(const std::vector<Vector3>& positions)
{
	assert((positions.size() & 1u) == 0 && "偶数個の座標が必要です");
	
	LineInstanceRenderer::GetInstance()->RegisterLine(this);
	lineIndex_ = LineInstanceRenderer::GetInstance()->GetLineID(this);

	MakeMaterialData();
}

void Line3d::Update()
{
	LineInstanceRenderer::GetInstance()->SetMaterial(this, materialData_);
}

void Line3d::MakeMaterialData()
{
	materialData_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_.enableDraw = true;
	materialData_.enableLighting = true;
	materialData_.outlineMask = false;
	materialData_.outlineSceneColor = false;
	materialData_.uvTransform = Matrix4x4::Identity();
	materialData_.shininess = 20.0f;
	materialData_.environmentCoefficient = 0;
}

void Line3d::SetPositions(const std::vector<Vector3>& positions)
{
	assert((positions.size() & 1u) == 0 && "偶数個の座標が必要です");

	LineInstanceRenderer::GetInstance()->SetLineInstances(this, positions);
}

void Line3d::SetColor(const Vector3& color)
{
	materialData_.color = { color.x,color.y,color.z,1.0f };
}

std::vector<Vector3> Line3d::CreateBox(const Vector3& min, const Vector3& max)
{
	std::vector<Vector3> linePosition = {
		min,
		{ max.x, min.y, min.z },

		min,
		{ min.x, max.y, min.z },

		min,
		{ min.x, min.y, max.z },

		{ min.x, max.y, min.z },
		{ max.x, max.y, min.z },

		{ min.x, max.y, min.z },
		{ min.x, max.y, max.z },

		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z },

		{ max.x, min.y, min.z },
		{ max.x, min.y, max.z },

		{ min.x, min.y, max.z },
		{ min.x, max.y, max.z },

		{ max.x, min.y, min.z },
		{ max.x, max.y, min.z },

		{ min.x, max.y, max.z },
		max,

		{ max.x, min.y, max.z },
		max,

		{ max.x, max.y, min.z },
		max,
	};

	return linePosition;
}

std::vector<Vector3> Line3d::CreateSphere(float radius)
{
	const int latitudeLines = 6;   // 緯線の分割数
	const int longitudeLines = 12;  // 経線の分割数

	std::vector<Vector3> linePosition;

	// 緯線（Z軸まわりのX-Y平面）
	for (int i = 0; i < latitudeLines; ++i) {
		float theta1 = (float)i / latitudeLines * std::numbers::pi_v<float>;
		float theta2 = (float)(i + 1) / latitudeLines * std::numbers::pi_v<float>;

		for (int j = 0; j < longitudeLines; ++j) {
			float phi = (float)j / longitudeLines * (std::numbers::pi_v<float> * 2.0f);

			Vector3 p1 = {
				radius * std::sin(theta1) * std::cos(phi),
				radius * std::cos(theta1),
				radius * std::sin(theta1) * std::sin(phi)
			};

			Vector3 p2 = {
				radius * std::sin(theta2) * std::cos(phi),
				radius * std::cos(theta2),
				radius * std::sin(theta2) * std::sin(phi)
			};

			linePosition.push_back(p1);
			linePosition.push_back(p2);
		}
	}

	// 経線（X軸またはY軸に固定した断面）
	for (int i = 0; i < longitudeLines; ++i) {
		float phi1 = (float)i / longitudeLines * (std::numbers::pi_v<float> * 2.0f);
		float phi2 = (float)(i + 1) / longitudeLines * (std::numbers::pi_v<float> *2.0f);

		for (int j = 0; j < latitudeLines; ++j) {
			float theta = (float)j / latitudeLines * std::numbers::pi_v<float>;

			Vector3 p1 = {
				radius * std::sin(theta) * std::cos(phi1),
				radius * std::cos(theta),
				radius * std::sin(theta) * std::sin(phi1)
			};

			Vector3 p2 = {
				radius * std::sin(theta) * std::cos(phi2),
				radius * std::cos(theta),
				radius * std::sin(theta) * std::sin(phi2)
			};

			linePosition.push_back(p1);
			linePosition.push_back(p2);
		}
	}

	return linePosition;
}

std::vector<Vector3> Line3d::CreateSegment(const Vector3& start, const Vector3& direction)
{
	std::vector<Vector3> linePosition;

	linePosition.push_back(start);
	linePosition.push_back(start + direction);

	return linePosition;
}

std::vector<Vector3> Line3d::CreateGrid(float cell, float size, float y)
{
	assert(cell > 0.0f && size > 0.0f && "cell と size は正の値で");

	// 線を引く範囲（±half で左右対称に敷く）
	const float half = size * 0.5f;

	// 端を含めて何本引くか
	const int lineCount = static_cast<int>(std::floor(size / cell)) + 1;

	std::vector<Vector3> pos;
	pos.reserve(lineCount * 4);     // “横線 + 縦線” で 4 頂点/本

	// Z 方向の横線（X 軸と平行）
	for (int i = 0; i < lineCount; ++i)
	{
		float z = -half + i * cell;
		pos.emplace_back(-half, y, z); // ← 左端
		pos.emplace_back(half, y, z); // → 右端
	}

	// X 方向の縦線（Z 軸と平行）
	for (int i = 0; i < lineCount; ++i)
	{
		float x = -half + i * cell;
		pos.emplace_back(x, y, -half); // ↑ 奥
		pos.emplace_back(x, y, half); // ↓ 手前
	}
	return pos;
}
