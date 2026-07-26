#include "Collider.h"

// Ensure correct project Vector/Quaternion types are visible
#include "../../Math/Structure/Vector3.h"
#include "../../Math/Structure/Quaternion.h"

#include "DirectXEngine.h"
#include "CollisionManager.h"

#include "Line3d.h"

NumaEngine::Collider::~Collider()
{
	CollisionManager::GetInstance()->RemoveCollider(this);
	if (line_ == nullptr) { return; }
	NumaEngine::DirectXEngine::GetSceneRenderer()->SetRemoveList(line_.get());
}

void NumaEngine::Collider::AddCollider()
{
	CollisionManager::GetInstance()->AddCollider(this);
}

void NumaEngine::Collider::RemoveCollider()
{
	CollisionManager::GetInstance()->RemoveCollider(this);
	if (line_ == nullptr) { return; }
	NumaEngine::DirectXEngine::GetSceneRenderer()->SetRemoveList(line_.get());
}

void NumaEngine::Collider::DrawCollider()
{
#ifdef ENABLE_EDITOR
	// Lineの初期化
	line_ = std::make_unique<NumaEngine::Line3d>();
	linePositions_ = CreateLinePositions();
	line_->Initialize(linePositions_);
#endif // ENABLE_EDITOR    
}

void NumaEngine::Collider::Update()
{
	if (line_ == nullptr) { return; }

	linePositions_ = CreateLinePositions();
	for (auto& linePos : linePositions_) {
		Matrix4x4 rotateMatrix = NumaEngine::Quaternion::MakeRotateMatrix(rotate_);
		NumaEngine::Vector3 translate = centerPosition_ + offsetPosition_.Transform(rotateMatrix);
		linePos = linePos.Transform(rotateMatrix) + translate;
	}
	line_->SetPositions(linePositions_);
	if (debugDrawEnable_) {
		line_->GetMaterial().enableDraw = isActive_;
	} else {
		line_->GetMaterial().enableDraw = debugDrawEnable_;
	}
	line_->Update();
}

void NumaEngine::Collider::LineUpdate()
{
	if (line_ == nullptr) { return; }

	linePositions_ = CreateLinePositions();
	line_->SetPositions(linePositions_);
	if (debugDrawEnable_) {
		line_->GetMaterial().enableDraw = isActive_;
	} else {
		line_->GetMaterial().enableDraw = debugDrawEnable_;
	}
	line_->Update();
}

void NumaEngine::Collider::SetColor(const NumaEngine::Vector3& color)
{
	if (line_ == nullptr) { return; }
	line_->SetColor(color);
}

void NumaEngine::Collider::SetDebugDrawEnable(bool flag)
{
	debugDrawEnable_ = flag;
}

std::vector<NumaEngine::Vector3> NumaEngine::Collider::CreateLinePositions()
{
	// Line用の座標を作成
	std::vector<NumaEngine::Vector3> linePositions;
    switch (myType_)
	{
	case NumaEngine::ColliderType::kSphere:
		linePositions = line_->CreateSphere(radius_);
		break;
	case NumaEngine::ColliderType::kSegment:
		linePositions = line_->CreateSegment(origin_, diff_);
		break;
	case NumaEngine::ColliderType::kOBB:
		linePositions = line_->CreateBox(-size_, size_);
		break;
	default:
		break;
	}

	return linePositions;
}

