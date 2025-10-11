#pragma once

#include <memory>

#include "Object3d.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

#include "objects/fieldObject/effect/FieldObjectEffect.h"

class FieldObject : public Object3d,Collider
{
public:

	void Init(SceneObject object);

	void Update();

private:

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	float RandomRange(float value);
	void UpdateShake(Vector3& shake);

private:

	std::unique_ptr<FieldObjectEffect> effect_ = nullptr;

	// シェイクしている時値が入っている
	Vector3 shake_;
	// オブジェクトの元の座標
	Vector3 centerPosition_;


};

