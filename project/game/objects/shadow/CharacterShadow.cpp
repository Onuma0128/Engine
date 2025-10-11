#include "CharacterShadow.h"

void CharacterShadow::Init(const WorldTransform& transform)
{
	Object3d::Initialize("shadow.obj");
	Object3d::SetSceneRenderer();

	transform_.parent_ = &transform;
	transform_.scale_ = { 0.5f,0.5f,0.45f };
}

void CharacterShadow::Init()
{
	Object3d::Initialize("shadow.obj");
	Object3d::SetSceneRenderer();

	transform_.scale_ = { 0.5f,0.5f,0.45f };
}

void CharacterShadow::Update()
{
	Object3d::Update();
}