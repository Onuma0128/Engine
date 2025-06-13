#include "FieldObject.h"

void FieldObject::Init(SceneObject object)
{
	Object3d::Initialize(object.fileName);
	Object3d::SetSceneRenderer();
	transform_ = object.transform;

	if (object.collider.active) {
		Collider::AddCollider();
		Collider::colliderName_ = "FieldObject";
		Collider::myType_ = object.collider.type;
		Collider::offsetPosition_ = object.collider.center;
		Collider::size_ = object.collider.size;
		Collider::DrawCollider();
	}
}

void FieldObject::Update()
{


	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Object3d::Update();
}