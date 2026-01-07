#pragma once

#include <memory>
#include <array>

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "PrimitiveDrawr.h"

/// <summary>
/// ボスの攻撃エフェクト
/// </summary>
enum class BossAttackEffect {
	Melee,
	JumpAttack,
	DashAttack,
};

// 前方宣言
class BossEnemy;

/// <summary>
/// ボスのエフェクトクラス
/// </summary>
class BossEffect
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ボスのポインタをセットする
	/// </summary>
	/// <param name="boss"></param>
	void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }

	/// <summary>
	/// 描画をするエフェクトをセットする
	/// </summary>
	/// <param name="effect"></param>
	void SetAttackEffect(BossAttackEffect effect);

	/// <summary>
	/// 攻撃範囲エフェクトをリセット
	/// </summary>
	void AttackEffectReset();

	/// <summary>
	/// ジャンプ時のエフェクトを呼び出す
	/// </summary>
	void OnceJumpEffect();

	/// <summary>
	/// ダウン時のエフェクトを呼び出す
	/// </summary>
	void OnceDownEffect();
	void EmitDownStar(bool flag);

private:

	/// <summary>
	/// 初期化
	/// </summary>
	void PrimitiveInit(std::unique_ptr<PrimitiveDrawr>& effect);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="effect"></param>
	void PrimitiveUpdate(std::unique_ptr<PrimitiveDrawr>& effect,
		const float time, const float size, const Vector3& offset);
	void PrimitiveUpdate(std::unique_ptr<PrimitiveDrawr>& effect,
		const float time, const Vector2& size, const Vector3& offset);

private:

	// ボス本体のポインタ
	BossEnemy* boss_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	// 攻撃エフェクト時間
	float attackEffectTime_ = 0.0f;

	/* ==================== ジャンプ攻撃時のエフェクト ==================== */

	std::shared_ptr<ParticleEmitter> jumpDustEmitter_ = nullptr;

	/* ==================== 近接攻撃の攻撃範囲エフェクト ==================== */

	std::unique_ptr<PrimitiveDrawr> meleeEffect_ = nullptr;
	std::unique_ptr<PrimitiveDrawr> meleeTimeEffect_ = nullptr;

	/* ==================== ジャンプ攻撃の攻撃範囲エフェクト ==================== */

	std::unique_ptr<PrimitiveDrawr> jumpAttackEffect_ = nullptr;
	std::unique_ptr<PrimitiveDrawr> jumpAttackTimeEffect_ = nullptr;

	/* ==================== ダッシュ攻撃の攻撃範囲エフェクト ==================== */

	std::unique_ptr<PrimitiveDrawr> dashAttackEffect_ = nullptr;
	std::unique_ptr<PrimitiveDrawr> dashAttackTimeEffect_ = nullptr;

	/* ==================== ダッシュ攻撃の後の怯みエフェクト ==================== */
	
	std::shared_ptr<ParticleEmitter> downLineEmitter_ = nullptr;
	std::shared_ptr<ParticleEmitter> downStarEmitter_ = nullptr;

};

