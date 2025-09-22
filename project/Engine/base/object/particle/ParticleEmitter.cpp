#include "ParticleEmitter.h"

#include "DirectXEngine.h"

#include "DeltaTimer.h"

ParticleEmitter::~ParticleEmitter()
{
    DirectXEngine::GetSceneRenderer()->SetRemoveList(line_.get());
}

ParticleEmitter::ParticleEmitter(const std::string name)
{
    emitter_.name = name;
    emitter_.copyName = name;
    emitter_.setPosition = { 0.0f,0.0f,0.0f };
    emitter_.transform = { {1.0f,1.0f,1.0f},Quaternion::IdentityQuaternion(),{0.0f,0.0f,0.0f} };
    emitter_.frequency = 0.1f;
    emitter_.frequencyTime = 0.0f;
    accelerationField_.acceleration = { 0.0f,10.0f,0.0f };
    accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
    accelerationField_.area.max = { 1.0f,1.0f,1.0f };
    // emitter_.frequencyごとに出すパーティクルの個数
    emitter_.count = 3;
    emitter_.emitterSize = {
        .min = {-1.0f,-1.0f,-1.0f},
        .max = {1.0f,1.0f,1.0f}
    };
    emitter_.minScale = { 1.0f,1.0f,1.0f };
    emitter_.maxScale = { 1.0f,1.0f,1.0f };

    emitter_.blendMode_ = 0;
    emitter_.isMoveStart = true;
    emitter_.isFieldStart = false;
    isCreate_ = true;
    needResetPrev_ = true;
    
#ifdef _DEBUG
    // Emitterの範囲を線で描画
    AABB aabb = emitter_.emitterSize;
    linePosition_ = CreateLineBox(emitter_.emitterSize);

    line_ = std::make_unique<Line3d>();
    line_->Initialize(linePosition_);
#endif // _DEBUG
}

void ParticleEmitter::Update()
{
    /*==================== パーティクルの範囲更新 ====================*/

    Emitter backup = emitter_;
    emitter_ = editorEmitter_;

    // コピーしない値を戻す
    emitter_.name = backup.name;
    emitter_.copyName = backup.copyName;
    emitter_.frequencyTime = backup.frequencyTime;
    emitter_.setPosition = backup.setPosition;
    emitter_.transform.scale = backup.transform.scale;
    emitter_.transform.rotation = backup.transform.rotation;

    // 新たな数値を代入
    Vector3 min = editorEmitter_.emitterSize.min;
    Vector3 max = editorEmitter_.emitterSize.max;
    // min,maxが最大値を超えていないかclamp
    emitter_.emitterSize.min = {
        std::clamp(min.x,-256.0f,0.0f),std::clamp(min.y,-256.0f,0.0f),std::clamp(min.z,-256.0f,0.0f),
    };
    emitter_.emitterSize.max = {
        std::clamp(max.x,0.0f,256.0f),std::clamp(max.y,0.0f,256.0f),std::clamp(max.z,0.0f,256.0f),
    };

    accelerationField_.acceleration = editorEmitter_.acceleration;

    Vector3 emitterPosition = editorEmitter_.transform.translation;
    if (!editorEmitter_.isLock) {
        emitter_.transform.translation = emitterPosition;
    } else {
        Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(emitter_.transform.rotation);
        emitter_.transform.translation = emitter_.setPosition + (emitterPosition).Transform(rotateMatrix);
        accelerationField_.acceleration = Vector3::Transform(accelerationField_.acceleration, rotateMatrix);
    }
    // Emitterの範囲より少し大きめにFieldを作る
    accelerationField_.area = {
        .min = emitter_.emitterSize.min + emitter_.transform.translation + editorEmitter_.minAccelerField,
        .max = emitter_.emitterSize.max + emitter_.transform.translation + editorEmitter_.maxAccelerField
    };

    // min,maxが最大値を超えていないかclamp
    min = editorEmitter_.minScale;
    max = editorEmitter_.maxScale;
    Vector3 end = editorEmitter_.endScale;
    emitter_.minScale = {
        std::clamp(min.x,-256.0f,max.x),std::clamp(min.y,-256.0f,max.y),std::clamp(min.z,-256.0f,max.z),
    };
    emitter_.maxScale = {
        std::clamp(max.x,min.x,256.0f),std::clamp(max.y,min.y,256.0f),std::clamp(max.z,min.z,256.0f),
    };
    emitter_.endScale = {
        std::clamp(end.x,0.0f,256.0f),std::clamp(end.y,0.0f,256.0f),std::clamp(end.z,0.0f,256.0f)
    };
    // min,maxが最大値を超えていないかclamp
    min = editorEmitter_.minVelocity;
    max = editorEmitter_.maxVelocity;
    emitter_.minVelocity = {
        std::clamp(min.x,-256.0f,max.x),std::clamp(min.y,-256.0f,max.y),std::clamp(min.z,-256.0f,max.z),
    };
    emitter_.maxVelocity = {
        std::clamp(max.x,min.x,256.0f),std::clamp(max.y,min.y,256.0f),std::clamp(max.z,min.z,256.0f),
    };

    // min,maxが最大値を超えていないかclamp
    min = editorEmitter_.minRotate;
    max = editorEmitter_.maxRotate;
    emitter_.minRotate = {
        std::clamp(min.x, -256.0f, max.x),std::clamp(min.y, -256.0f, max.y),std::clamp(min.z, -256.0f, max.z)
    };
    emitter_.maxRotate = {
        std::clamp(max.x, min.x, 256.0f),std::clamp(max.y, min.y, 256.0f),std::clamp(max.z, min.z, 256.0f)
    };
    // min,maxが最大値を超えていないかclamp
    min = editorEmitter_.minRotateSpeed;
    max = editorEmitter_.maxRotateSpeed;
    emitter_.minRotateSpeed = {
        std::clamp(min.x, -256.0f, max.x),std::clamp(min.y, -256.0f, max.y),std::clamp(min.z, -256.0f, max.z)
    };
    emitter_.maxRotateSpeed = {
        std::clamp(max.x, min.x, 256.0f),std::clamp(max.y, min.y, 256.0f),std::clamp(max.z, min.z, 256.0f)
    };

#ifdef _DEBUG

    linePosition_ = CreateLineBox(emitter_.emitterSize);
    for (auto& linePos : linePosition_) {
        Vector3 translate = emitter_.transform.translation;
        Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(emitter_.transform.rotation);
        linePos = linePos.Transform(rotateMatrix) + translate;
    }
    line_->SetPositions(linePosition_);
    line_->Update();

#endif // _DEBUG
}

void ParticleEmitter::CreateParticles(ParticleManager::ParticleGroup& group)
{
    if (emitter_.isMoveStart && isCreate_) {
        emitter_.frequencyTime += DeltaTimer::GetDeltaTime();
        if (emitter_.frequency <= emitter_.frequencyTime) {
            std::mt19937 randomEngine_(seedGenerator_());
            group.particles.splice(group.particles.end(), Emit(emitter_, randomEngine_));
            emitter_.frequencyTime -= emitter_.frequency;

            // 最初の一回目は補完をしない
            if (needResetPrev_ && !hasPrevPos_) {
                prevPosition_ = emitter_.transform.translation;
                hasPrevPos_ = true;
            }
            // 移動量が多いときその移動の補完をする
            float dist = (emitter_.transform.translation - prevPosition_).Length();
            Vector3 span = emitter_.emitterSize.max - emitter_.emitterSize.min;
            float  threshold = span.Length();
            int n = static_cast<int>(dist / threshold);
            for (int i = 1; i <= n; ++i) {
                float t = static_cast<float>(i) / (n + 1);
                Vector3 pos = Vector3::Lerp(prevPosition_, emitter_.transform.translation, t);

                Transform3D backup = emitter_.transform;          // 元を退避
                emitter_.transform.translation = pos;             // 中間点に置き換える
                {
                    std::mt19937 rng(seedGenerator_());
                    group.particles.splice(group.particles.end(), Emit(emitter_, rng));
                }
                emitter_.transform = backup;
            }

            // 前の座標を保存
            if (needResetPrev_) {
                prevPosition_ = emitter_.transform.translation;
                needResetPrev_ = false;
            }
        }
    } else {
        hasPrevPos_ = false;
        needResetPrev_ = true;
    }

    if (onceEmit_) {
        onceEmit_ = false;
        std::mt19937 randomEngine_(seedGenerator_());
        group.particles.splice(group.particles.end(), Emit(emitter_, randomEngine_));
    }
}

void ParticleEmitter::UpdateParticle(std::list<ParticleManager::Particle>::iterator& particle)
{
    if (emitter_.isMoveStart) {
        if (IsCollision(accelerationField_.area, particle->transform.translation) && emitter_.isFieldStart) {
            particle->velocity += accelerationField_.acceleration * DeltaTimer::GetDeltaTime();
        }
        particle->transform.translation += particle->velocity * DeltaTimer::GetDeltaTime();
        // 反射の処理
        if (emitter_.isReflect) {
            if (particle->transform.translation.y <= emitter_.reflectY) {
                particle->velocity.y *= -(3.0f / 4.0f);
            }
        }
        // スケールの処理
        if (emitter_.isChangeScale) {
            float t = 1.0f - (particle->currentTime / particle->lifeTime);
            particle->transform.scale = t * particle->offsetScale + (1.0f - t) * emitter_.endScale;
        }
        particle->transform.rotation += particle->rotateSpeed * DeltaTimer::GetDeltaTime();
        particle->uvTranslate += emitter_.uvTranslation * DeltaTimer::GetDeltaTime();

        particle->currentTime += DeltaTimer::GetDeltaTime();

        particle->color.x = emitter_.color.x;
        particle->color.y = emitter_.color.y;
        particle->color.z = emitter_.color.z;
    }
}

std::list<ParticleManager::Particle> ParticleEmitter::Emit(const Emitter& emitter, std::mt19937& randomEngine)
{
    std::list<ParticleManager::Particle> particles;
    for (uint32_t count = 0; count < emitter.count; ++count) {
        particles.push_back(MakeNewParticle(randomEngine, emitter));
    }
    return particles;
}

ParticleManager::Particle ParticleEmitter::MakeNewParticle(std::mt19937& randomEngine, const Emitter& emitter)
{
    std::uniform_real_distribution<float> distPosX(emitter.emitterSize.min.x, emitter.emitterSize.max.x);
    std::uniform_real_distribution<float> distPosY(emitter.emitterSize.min.y, emitter.emitterSize.max.y);
    std::uniform_real_distribution<float> distPosZ(emitter.emitterSize.min.z, emitter.emitterSize.max.z);

    std::uniform_real_distribution<float> distScaleX(emitter.minScale.x, emitter.maxScale.x);
    std::uniform_real_distribution<float> distScaleY(emitter.minScale.y, emitter.maxScale.y);
    std::uniform_real_distribution<float> distScaleZ(emitter.minScale.z, emitter.maxScale.z);

    std::uniform_real_distribution<float> distRotateX(emitter.minRotate.x, emitter.maxRotate.x);
    std::uniform_real_distribution<float> distRotateY(emitter.minRotate.y, emitter.maxRotate.y);
    std::uniform_real_distribution<float> distRotateZ(emitter.minRotate.z, emitter.maxRotate.z);

    std::uniform_real_distribution<float> distRotateSpeedX(emitter.minRotateSpeed.x, emitter.maxRotateSpeed.x);
    std::uniform_real_distribution<float> distRotateSpeedY(emitter.minRotateSpeed.y, emitter.maxRotateSpeed.y);
    std::uniform_real_distribution<float> distRotateSpeedZ(emitter.minRotateSpeed.z, emitter.maxRotateSpeed.z);

    ParticleManager::Particle particle{};
    particle.transform.scale = { distScaleX(randomEngine),distScaleY(randomEngine),distScaleZ(randomEngine) };
    particle.offsetScale = particle.transform.scale;
    if (emitter.isBillboard) {
        particle.transform.rotation = { 0.0f,0.0f,distRotateZ(randomEngine) };
        particle.rotateSpeed = { 0.0f,0.0f,distRotateSpeedZ(randomEngine) };
    } else {
        particle.transform.rotation = { distRotateX(randomEngine),distRotateY(randomEngine),distRotateZ(randomEngine) };
        particle.rotateSpeed = { distRotateSpeedX(randomEngine),distRotateSpeedY(randomEngine),distRotateSpeedZ(randomEngine) };
    }
    Vector3 randomTranslate = { distPosX(randomEngine),distPosY(randomEngine) ,distPosZ(randomEngine) };
    Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(emitter.transform.rotation);
    particle.transform.translation = emitter.transform.translation + randomTranslate.Transform(rotateMatrix);
    
    if (!emitter.isLockDirection) {
        std::uniform_real_distribution<float> distVelocityX(emitter.minVelocity.x, emitter.maxVelocity.x);
        std::uniform_real_distribution<float> distVelocityY(emitter.minVelocity.y, emitter.maxVelocity.y);
        std::uniform_real_distribution<float> distVelocityZ(emitter.minVelocity.z, emitter.maxVelocity.z);

        Vector3 velocity = { distVelocityX(randomEngine),distVelocityY(randomEngine),distVelocityZ(randomEngine) };
        particle.velocity = velocity.Transform(Quaternion::MakeRotateMatrix(emitter.transform.rotation));
    } else {
        Vector3 d = emitter.transform.translation - particle.transform.translation;
        d.z = 0.0f; // XY平面に限定
        float len2 = d.x * d.x + d.y * d.y;
        if (len2 < 1e-12f) { d = Vector3::ExprUnitY; } 
        else { d = d * (1.0f / std::sqrt(len2)); }
        float roll = std::atan2(-d.x, d.y);
        particle.transform.rotation = { 0.0f, 0.0f, roll };
        Vector3 forwardXY = { -std::sin(roll), std::cos(roll), 0.0f };
        particle.velocity = forwardXY * emitter.directionSpeed;
        particle.rotateSpeed = Vector3::ExprZero;
    }

    particle.color = { 1.0f,1.0f,1.0f,1.0f };
    particle.lifeTime = emitter.lifeTime;
    particle.currentTime = 0.0f;
    particle.emitterName = emitter.copyName;
    return particle;
}

std::vector<Vector3> ParticleEmitter::CreateLineBox(AABB aabb)
{
    std::vector<Vector3> linePosition = {
        aabb.min,
        { aabb.max.x, aabb.min.y, aabb.min.z },

        aabb.min,
        { aabb.min.x, aabb.max.y, aabb.min.z },

        aabb.min,
        { aabb.min.x, aabb.min.y, aabb.max.z },

        { aabb.min.x, aabb.max.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.min.z },

        { aabb.min.x, aabb.max.y, aabb.min.z },
        { aabb.min.x, aabb.max.y, aabb.max.z },

        { aabb.min.x, aabb.min.y, aabb.max.z },
        { aabb.max.x, aabb.min.y, aabb.max.z },

        { aabb.max.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.min.y, aabb.max.z },

        { aabb.min.x, aabb.min.y, aabb.max.z },
        { aabb.min.x, aabb.max.y, aabb.max.z },

        { aabb.max.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.min.z },

        { aabb.min.x, aabb.max.y, aabb.max.z },
        aabb.max,

        { aabb.max.x, aabb.min.y, aabb.max.z },
        aabb.max,

        { aabb.max.x, aabb.max.y, aabb.min.z },
        aabb.max,
    };

    return linePosition;
}


bool ParticleEmitter::IsCollision(const AABB& aabb, const Vector3& point)
{
    if (aabb.min.x < point.x && aabb.max.x > point.x &&
        aabb.min.y < point.y && aabb.max.y > point.y &&
        aabb.min.z < point.z && aabb.max.z > point.z) {
        return true;
    }
    return false;
}