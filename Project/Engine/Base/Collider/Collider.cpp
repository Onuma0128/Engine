#include "Collider.h"

#include "DirectXEngine.h"

#include "Line3d.h"

Collider::~Collider()
{
	DirectXEngine::GetCollisionMgr()->RemoveCollider(this);
	if (line_ == nullptr) { return; }
	DirectXEngine::GetSceneRenderer()->SetRemoveList(line_.get());
}

void Collider::AddCollider()
{
	DirectXEngine::GetCollisionMgr()->AddCollider(this);
}

void Collider::RemoveCollider()
{
	DirectXEngine::GetCollisionMgr()->RemoveCollider(this);
	if (line_ == nullptr) { return; }
	DirectXEngine::GetSceneRenderer()->SetRemoveList(line_.get());
}

void Collider::DrawCollider()
{
#ifdef ENABLE_EDITOR
	// Lineの初期化
	line_ = std::make_unique<Line3d>();
	linePositions_ = CreateLinePositions();
	line_->Initialize(linePositions_);
#endif // ENABLE_EDITOR	
}

void Collider::Update()
{
	if (line_ == nullptr) { return; }

	linePositions_ = CreateLinePositions();
	for (auto& linePos : linePositions_) {
		Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotate_);
		Vector3 translate = centerPosition_ + offsetPosition_.Transform(rotateMatrix);
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

void Collider::LineUpdate()
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

void Collider::SetColor(const Vector3& color)
{
	if (line_ == nullptr) { return; }
	line_->SetColor(color);
}

void Collider::SetDebugDrawEnable(bool flag)
{
	debugDrawEnable_ = flag;
}

std::vector<Vector3> Collider::CreateLinePositions()
{
	// Line用の座標を作成
	std::vector<Vector3> linePositions;
	switch (myType_)
	{
	case ColliderType::kSphere:
		linePositions = line_->CreateSphere(radius_);
		break;
	case ColliderType::kSegment:
		linePositions = line_->CreateSegment(origin_, diff_);
		break;
	case ColliderType::kOBB:
		linePositions = line_->CreateBox(-size_, size_);
		break;
	default:
		break;
	}

	return linePositions;
}
