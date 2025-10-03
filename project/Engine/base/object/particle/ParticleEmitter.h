#pragma once

#include <iostream>
#include <random>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>

#include "Line3d.h"

#include "Transform.h"
#include "Vector3.h"

#include "ParticleManager.h"

class ParticleEmitter
{
public:

	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	struct Emitter {

		/* ==================== Emitter ==================== */

		std::string name;			// 何のParticleを保持しているかのname
		uint16_t id;				// Particleのコピーが出来た時のid
		Vector3 setPosition;		// Emitterを固定する座標
		Transform3D transform;		// EmitterのTransform
		AABB emitterSize;			// Emitterのsize

		/* ==================== Particle ==================== */

		std::string texture;		// ParticleのTexture
		uint32_t model_;			// ParticleのModel
		uint32_t blendMode_;		// ParticleのBlendModeを設定
		Vector3 minScale;			// Particle発生時の最小size
		Vector3 maxScale;			// Particle発生時の最大size
		Vector3 endScale;			// Particleが消える時のsize

		Vector3 minVelocity;		// Particle発生時の最小velocity
		Vector3 maxVelocity;		// Particle発生時の最大velocity
		float directionSpeed;		// Particle発生時のSpeed

		Vector3 acceleration;		// Particleの全体の加速度
		Vector3 minAccelerField;	// 加速度が掛かるフィールドの最小size
		Vector3 maxAccelerField;	// 加速度が掛かるフィールドの最大size

		Vector3 minRotate;			// Particle発生時の最小rotate
		Vector3 maxRotate;			// Particle発生時の最大rotate
		Vector3 minRotateSpeed;		// Particle発生時の最小rotateSpeed
		Vector3 maxRotateSpeed;		// Particle発生時の最大rotateSpeed
		float reflectY;				// Particleが反射するY軸を設定

		Vector3 uvTranslation;		// ParticleのUVをスクロールする

		uint32_t count;				// Particle発生時に何個出すか
		float lifeTime;				// Particle発生時から生きている時間
		float frequency;			// Particle発生するまでの時間
		float frequencyTime;		// Particle発生するまでカウントするTime
		Vector3 color;

		bool isLock;				// Emitterを固定するか
		bool isMoveStart;			// Particleを動かすか
		bool isLockDirection;		// Particle発生時のvelocityを固定
		bool isFieldStart;			// ParticleにAccelerationを掛けるか
		bool isReflect;				// Y軸の高さで反射をするか
		bool isChangeScale;			// 生きている時間でScaleを変えるか
		bool isBillboard;			// ビルボードしているか
		bool isUvFlipX;				// UvFlipX
		bool isUvFlipY;				// UvFlipY
	};

	struct AccelerationField {
		Vector3 acceleration;
		AABB area;
	};

public:

	~ParticleEmitter();
	ParticleEmitter(const std::string name);

	void Update();

	/*==================== パーティクルの生成 ====================*/

	void CreateParticles(ParticleManager::ParticleGroup& group);

	void UpdateParticle(std::list<ParticleManager::Particle>::iterator& particle);

	// 呼び出す事で一回だけEmitする
	void onceEmit() { 
		onceEmit_ = true;
	}

	/*==================== アクセッサー ====================*/

	void SetEmitter(const Emitter& emitter) { editorEmitter_ = emitter; }

	void SetRotation(const Quaternion& rotation) { emitter_.transform.rotation = rotation; }

	void SetPosition(const Vector3& position) { 
		emitter_.setPosition = position;
		needResetPrev_ = true;
	}

	void SetAcceleration(const Vector3& acceleration) { accelerationField_.acceleration = acceleration; }

	void SetIsCreate(bool isCreate) { isCreate_ = isCreate; }

	int GetBlendMode()const { return emitter_.blendMode_; }

	void SetId(uint16_t id) { emitter_.id = id; }
	uint16_t GetId()const { return emitter_.id; }
	std::string GetName()const { return emitter_.name; }

	void SetDrawLine(bool flag) { line_->GetMaterial().enableDraw = flag; }

private:

	static std::list<ParticleManager::Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine);

	static ParticleManager::Particle MakeNewParticle(std::mt19937& randomEngine, const Emitter& emitter);

	std::vector<Vector3> CreateLineBox(AABB aabb);

	bool IsCollision(const AABB& aabb, const Vector3& point);

private:

	/*==================== エミッター ====================*/

	// 乱数生成器の初期化
	std::random_device seedGenerator_;

	// エミッタの範囲を可視化
	std::vector<Vector3> linePosition_;
	std::unique_ptr<Line3d> line_;

	// Transform変数を作る
	AccelerationField accelerationField_{};
	Emitter emitter_{};
	Emitter editorEmitter_{};
	// emitterの前の座標を保存する
	bool hasPrevPos_ = false;
	bool needResetPrev_ = false;
	Vector3 prevPosition_;

	bool isCreate_ = true;    
	bool onceEmit_ = false;
};