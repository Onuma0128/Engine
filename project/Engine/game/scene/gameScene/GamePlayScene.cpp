#include "GamePlayScene.h"

#include "DirectXEngine.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "ModelManager.h"

#include "Collision3D.h"

void GamePlayScene::Initialize()
{
	demoObj_ = std::make_unique<Object3d>();
	demoObj_->Initialize("plane.obj");
	demoObj_->SetTexture("resources", "white1x1.png");
	demoObj_->SetColor({ 0.1f,0.1f,0.1f,1.0f });
	demoObj_->GetTransform().scale_ = { 20.0f,20.0f ,20.0f };
	demoObj_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, -1.57f);

	player_ = std::make_unique<Player>();
	player_->Init();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init();

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();

	DirectXEngine::GetPostEffectMgr()->CreatePostEffect(PostEffectType::Grayscale);
	DirectXEngine::GetPostEffectMgr()->CreatePostEffect(PostEffectType::Vignette);
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	demoObj_->Update();

	player_->Update();

	enemy_->Update();

	gameCamera_->Update();

	Collision();

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
	demoObj_->Draw();

	player_->Draw();

	enemy_->Draw();

	ParticleManager::GetInstance()->Draw();
}

void GamePlayScene::Collision()
{
	for (auto& bullet : player_->GetBullets()) {
		if (!bullet->GetIsActive()) { continue; }
		// 敵のOBB
		OBB obb1 = {
			.center = enemy_->GetTransform().translation_,
			.rotateMatrix = Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_),
			.size = enemy_->GetTransform().scale_
		};
		// 弾のSphere
		Sphere sphere = {
			.center = bullet->GetTransform().translation_,
			.radius = bullet->GetTransform().scale_.x
		};

		std::string name;
		bool isCollision = Collision3D::CollisionChecker(obb1, sphere, name);

		if (isCollision) { 
			enemy_->GetEffect()->OnceBulletHitEffect(bullet->GetTransform());
			bullet->IsCollision();
			gameCamera_->SetShake(1.0f);
		}
	}
}
