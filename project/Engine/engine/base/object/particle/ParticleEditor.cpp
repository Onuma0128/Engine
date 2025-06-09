#include "ParticleEditor.h"


void ParticleEditor::Initialize(std::string filePath)
{
	parameters_.Init(filePath);

	if (!parameters_.Load()) {

		/* ==================== Particle全体の固定値 ==================== */

		// Texture
		parameters_.Set("Texture", "white1x1.png");
		// BlendMode
		parameters_.Set("BlendMode", 0);
		// 発生数
		parameters_.Set("EmitCount", 0);
		// 発生までの時間
		parameters_.Set("Move", false);
		parameters_.Set("FrequencyTime", 0.1f);
		// 発生してから消えるまでの時間
		parameters_.Set("LifeTime", 0.5f);
		// パーティクルが反射するY軸
		parameters_.Set("Reflect", false);
		parameters_.Set("ReflectY", 0.0f);

		/* ==================== Particle1つのランダム値 ==================== */

		// スケール
		parameters_.Set("MinScale", Vector3{});
		parameters_.Set("MaxScale", Vector3{});
		parameters_.Set("ChangeScale", false);
		parameters_.Set("EndScale", Vector3{});
		// 速度
		parameters_.Set("MinVelocity", Vector3{});
		parameters_.Set("MaxVelocity", Vector3{});
		// 加速度
		parameters_.Set("Acceleration", Vector3{});
		// 回転
		parameters_.Set("Billboard", true);
		parameters_.Set("MinRotate", Vector3{});
		parameters_.Set("MaxRotate", Vector3{});
		parameters_.Set("MinRotateSpeed", Vector3{});
		parameters_.Set("MaxRotateSpeed", Vector3{});
		
		/* ==================== Emitterの固定値 ==================== */

		// 加速度が掛かるフィールドのスケール
		parameters_.Set("AcceleField", false);
		parameters_.Set("AccelerationField", Vector3{});
		// Transform
		parameters_.Set("Scale", Vector3{});
		parameters_.Set("Rotate", Vector3{});
		parameters_.Set("Position", Vector3{});
		// Rotate,Positionを固定するか
		parameters_.Set("Look", false);
	}
}

void ParticleEditor::Update(ParticleEmitter::Emitter emitter)
{

}