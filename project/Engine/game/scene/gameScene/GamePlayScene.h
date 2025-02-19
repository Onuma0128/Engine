#pragma once
#include <memory>

#include "BaseScene.h"

#include "Object3d.h"
#include "WorldTransform.h"
#include "Sprite.h"
#include "ParticleEmitter.h"
#include "TrailEffect.h"

#include "gameScene/test/TestObject.h"

class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

	void AddQuaternion(Quaternion& quaternion, const Vector3& angle);

private:

	std::unique_ptr<TestObject> testObj_ = nullptr;

	std::unique_ptr<Object3d> terrain_ = nullptr;
	Vector3 terrainAngle_;

	std::unique_ptr<Object3d> sphere_ = nullptr;
	Vector3 sphereAngle_;

	std::unique_ptr<Object3d> plane_ = nullptr;
	Vector3 planeAngle_;

	std::unique_ptr<Sprite> sprite_ = nullptr;

	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;

	std::unique_ptr<TrailEffect> sphereEffect_ = nullptr;

};

