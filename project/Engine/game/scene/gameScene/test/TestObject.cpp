#include "TestObject.h"

#include "ModelManager.h"

void TestObject::Init()
{
	Object3d::Initialize("terrain.obj");
}

void TestObject::Update()
{
	Object3d::Update();
}

void TestObject::Draw()
{
	Object3d::Draw();
}
