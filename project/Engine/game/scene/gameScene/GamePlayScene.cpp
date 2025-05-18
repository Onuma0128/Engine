#include "GamePlayScene.h"

#include "ParticleManager.h"
#include "SceneManager.h"

#include "gameScene/enemy/Enemy.h"

#include "Collision3D.h"
#include "Vector3.h"

void GamePlayScene::Initialize()
{
	demoObj_ = std::make_unique<Object3d>();
	demoObj_->Initialize("terrain.obj");
	//demoObj_->SetTexture("resources", "uvChecker.png");
	//demoObj_->SetColor({ 0.1f,0.1f,0.1f,1.0f });
	//demoObj_->GetTransform().scale_ = { 20.0f,20.0f,20.0f };
	//demoObj_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, -1.57f);

	skyBox_ = std::make_unique<PrimitiveDrawr>();
	skyBox_->TypeInit(PrimitiveType::Skybox);
	skyBox_->GetTransform().scale = { 512.0f,512.0f ,512.0f };
	skyBox_->GetRenderOptions().enabled = false;

	player_ = std::make_unique<Player>();
	player_->Init();

	enemySpawner_ = std::make_unique<EnemySpawner>();
	enemySpawner_->Init();

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	demoObj_->Update();

	player_->Update();

	enemySpawner_->Update();

	gameCamera_->Update();

	Collision();

	skyBox_->Update();

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
}

void GamePlayScene::Collision()
{
	//for (auto& bullet : player_->GetBullets()) {
	//	if (!bullet->GetIsActive()) { continue; }
	//	for (auto& enemy : enemySpawner_->GetEnemyList()) {
	//		// 敵のOBB
	//		OBB obb1 = {
	//			.center = enemy->GetTransform().translation_,
	//			.rotateMatrix = Quaternion::MakeRotateMatrix(enemy->GetTransform().rotation_),
	//			.size = enemy->GetTransform().scale_
	//		};
	//		// 弾のSphere
	//		Sphere sphere = {
	//			.center = bullet->GetTransform().translation_,
	//			.radius = bullet->GetTransform().scale_.x
	//		};

	//		std::string name;
	//		bool isCollision = Collision3D::CollisionChecker(obb1, sphere, name);

	//		if (isCollision) {
	//			enemy->GetEffect()->OnceBulletHitEffect(bullet->GetTransform());
	//			bullet->IsCollision();
	//			gameCamera_->SetShake(1.0f);
	//		}
	//	}
	//}

	//OBB obb = {
	//	.center = enemySpawner_->GetTransform().translation_,
	//	.rotateMatrix = Quaternion::MakeRotateMatrix(enemySpawner_->GetTransform().rotation_),
	//	.size = enemySpawner_->GetTransform().scale_
	//};

	//if (Collision3D::CollisionChecker(obb, player_->GetReticle()->GetSegment())) {
	//	player_->GetReticle()->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	//} else {
	//	player_->GetReticle()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	//}
}
