#include "BossAdjustItem.h"

#include "imgui.h"

void BossAdjustItem::LoadItems()
{
	/* ============================== Main ============================== */
	mainJson_.Init("BossMain");
	if (!mainJson_.Load()) {
		mainJson_.Set("maxHP", 20);
		mainJson_.Set("speed", 2.0f);
        mainJson_.Set("rayDistance", 2.0f);
        mainJson_.Set("rayOffset", Vector3{});
        mainJson_.Set("colliderSize", 0.0f);
        mainJson_.Set("colliderOffset", Vector3{});
		mainJson_.Set("searchUpdateTime", 0.0f);
	} else {
		mainData_.maxHP = mainJson_.Get("maxHP", mainData_.maxHP);
		mainData_.speed = mainJson_.Get("speed", mainData_.speed);
        mainData_.rayDistance = mainJson_.Get("rayDistance", mainData_.rayDistance);
        mainData_.rayOffset = mainJson_.Get("rayOffset", mainData_.rayOffset);
        mainData_.colliderSize = mainJson_.Get("colliderSize", mainData_.colliderSize);
        mainData_.colliderOffset = mainJson_.Get("colliderOffset", mainData_.colliderOffset);
		mainData_.searchUpdateTime = mainJson_.Get("searchUpdateTime", mainData_.searchUpdateTime);
	}
	mainData_.debugSpline = false;

	/* ============================== Spawn ============================== */
	spawnJson_.Init("BossEnemySpawn");
	spawnData_.positions.clear();

	if (!spawnJson_.Load()) {
		spawnData_.maxSpawnCount = 0;
		spawnJson_.Set("maxSpawnCount", spawnData_.maxSpawnCount);
		spawnJson_.Set("position0", Vector3{});
	} else {
		spawnData_.maxSpawnCount = spawnJson_.Get("maxSpawnCount", spawnData_.maxSpawnCount);
		spawnData_.positions.resize(spawnData_.maxSpawnCount);
		for (int i = 0; i < spawnData_.maxSpawnCount; ++i) {
			spawnData_.positions[i] = spawnJson_.Get("position" + std::to_string(i), Vector3{});
		}
	}

	savedSpawnCount_ = spawnData_.maxSpawnCount;

    // ============================== BossMeleeAttack ==============================
    meleeAttackJson_.Init("BossMeleeAttack");

    if (!meleeAttackJson_.Load())
    {
        // 保存
        meleeAttackJson_.Set("attackColliderSize", meleeData_.attackColliderSize);
        meleeAttackJson_.Set("attackColliderOffset", meleeData_.attackColliderOffset);
        meleeAttackJson_.Set("attackStartupTime", meleeData_.attackStartupTime);
        meleeAttackJson_.Set("attackTime", meleeData_.attackTime);
        meleeAttackJson_.Set("attackRecoveryTime", meleeData_.attackRecoveryTime);
    } else
    {
        // 読み込み
        meleeData_.attackColliderSize = meleeAttackJson_.Get("attackColliderSize", meleeData_.attackColliderSize);
        meleeData_.attackColliderOffset = meleeAttackJson_.Get("attackColliderOffset", meleeData_.attackColliderOffset);
        meleeData_.attackStartupTime = meleeAttackJson_.Get("attackStartupTime", meleeData_.attackStartupTime);
        meleeData_.attackTime = meleeAttackJson_.Get("attackTime", meleeData_.attackTime);
        meleeData_.attackRecoveryTime = meleeAttackJson_.Get("attackRecoveryTime", meleeData_.attackRecoveryTime);
    }

    /* ============================== JumpAttack ============================== */
    jumpAttackJson_.Init("BossJumpAttack");
    if (!jumpAttackJson_.Load()) {
        jumpAttackJson_.Set("airSpeed", 0.0f);
        jumpAttackJson_.Set("jumpVelocityY", 0.0f);
        jumpAttackJson_.Set("jumpAccelerY", 0.0f);
        jumpAttackJson_.Set("airHoldTime", 0.0f);
        jumpAttackJson_.Set("attackColliderSize", 0.0f);
        jumpAttackJson_.Set("attackColliderOffset", Vector3{});
        jumpAttackJson_.Set("attackStartupTime", 0.0f);
        jumpAttackJson_.Set("jumpUpTime", 0.0f);
        jumpAttackJson_.Set("airHoldTime", 0.0f);
        jumpAttackJson_.Set("fallDownTime", 0.0f);
        jumpAttackJson_.Set("attackRecoveryTime", 0.0f);
    } else {
        jumpAttackData_.airSpeed = jumpAttackJson_.Get("airSpeed", jumpAttackData_.airSpeed);
        jumpAttackData_.jumpVelocityY = jumpAttackJson_.Get("jumpVelocityY", jumpAttackData_.jumpVelocityY);
        jumpAttackData_.jumpAccelerY = jumpAttackJson_.Get("jumpAccelerY", jumpAttackData_.jumpAccelerY);
        jumpAttackData_.attackColliderSize = jumpAttackJson_.Get("attackColliderSize", jumpAttackData_.attackColliderSize);
        jumpAttackData_.attackColliderOffset = jumpAttackJson_.Get("attackColliderOffset", jumpAttackData_.attackColliderOffset);
        jumpAttackData_.attackStartupTime = jumpAttackJson_.Get("attackStartupTime", jumpAttackData_.attackStartupTime);
        jumpAttackData_.JumpUpTime = jumpAttackJson_.Get("jumpUpTime", jumpAttackData_.JumpUpTime);
        jumpAttackData_.airHoldTime = jumpAttackJson_.Get("airHoldTime", jumpAttackData_.airHoldTime);
        jumpAttackData_.fallDownTime = jumpAttackJson_.Get("fallDownTime", jumpAttackData_.fallDownTime);
        jumpAttackData_.attackRecoveryTime = jumpAttackJson_.Get("attackRecoveryTime", jumpAttackData_.attackRecoveryTime);
    }

    /* ============================== BossDashAttack ============================== */
    dashAttackJson_.Init("BossDashAttack");
    if (!dashAttackJson_.Load())
    {
        dashAttackJson_.Set("dashSpeed", dashAttackData_.dashSpeed);
        dashAttackJson_.Set("attackColliderSize", dashAttackData_.attackColliderSize);
        dashAttackJson_.Set("attackColliderOffset", dashAttackData_.attackColliderOffset);
        dashAttackJson_.Set("attackStartupTime", dashAttackData_.attackStartupTime);
        dashAttackJson_.Set("dashTime", dashAttackData_.dashTime);
        dashAttackJson_.Set("attackRecoveryTime", dashAttackData_.attackRecoveryTime);
    } else
    {
        dashAttackData_.dashSpeed = dashAttackJson_.Get("dashSpeed", dashAttackData_.dashSpeed);
        dashAttackData_.attackColliderSize = dashAttackJson_.Get("attackColliderSize", dashAttackData_.attackColliderSize);
        dashAttackData_.attackColliderOffset = dashAttackJson_.Get("attackColliderOffset", dashAttackData_.attackColliderOffset);
        dashAttackData_.attackStartupTime = dashAttackJson_.Get("attackStartupTime", dashAttackData_.attackStartupTime);
        dashAttackData_.dashTime = dashAttackJson_.Get("dashTime", dashAttackData_.dashTime);
        dashAttackData_.attackRecoveryTime = dashAttackJson_.Get("attackRecoveryTime", dashAttackData_.attackRecoveryTime);
    }
}


void BossAdjustItem::Editor()
{
    ImGui::Begin("GameData");
    if (ImGui::TreeNode("BossParameters")) {
        ImGui::Separator();
        ImGui::PushItemWidth(150);

        // メイン項目
        if (ImGui::TreeNode("Main")) {
            ImGui::DragInt("maxHP", &mainData_.maxHP, 1, 1, 1000);
            ImGui::DragFloat("speed", &mainData_.speed, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("rayDistance", &mainData_.rayDistance, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("rayOffset", &mainData_.rayOffset.x, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("colliderSize", &mainData_.colliderSize, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("colliderOffset", &mainData_.colliderOffset.x, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("searchUpdateTime", &mainData_.searchUpdateTime, 0.01f, 0.0f, 100.0f);
            ImGui::Checkbox("debugSpline", &mainData_.debugSpline);

            if (ImGui::Button("Save")) {
                mainJson_.Set("maxHP", mainData_.maxHP);
                mainJson_.Set("speed", mainData_.speed);
                mainJson_.Set("rayDistance", mainData_.rayDistance);
                mainJson_.Set("rayOffset", mainData_.rayOffset);
                mainJson_.Set("colliderSize", mainData_.colliderSize);
                mainJson_.Set("colliderOffset", mainData_.colliderOffset);
                mainJson_.Set("searchUpdateTime", mainData_.searchUpdateTime);
                mainJson_.Save();
            }
            ImGui::TreePop();
        }
        ImGui::Separator();

        // 敵スポーン
        if (ImGui::TreeNode("EnemySpawn")) {
            ImGui::Text("maxSpawnCount: %d", spawnData_.maxSpawnCount);
            ImGui::SameLine();
            if (ImGui::Button("-")) {
                if (spawnData_.maxSpawnCount > 0) {
                    spawnData_.maxSpawnCount--;
                    if ((int)spawnData_.positions.size() > spawnData_.maxSpawnCount) {
                        spawnData_.positions.resize(spawnData_.maxSpawnCount);
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("+")) {
                spawnData_.maxSpawnCount++;
                if ((int)spawnData_.positions.size() < spawnData_.maxSpawnCount) {
                    spawnData_.positions.resize(spawnData_.maxSpawnCount, Vector3{});
                }
            }

            ImGui::Separator();

            for (int i = 0; i < spawnData_.maxSpawnCount; ++i) {
                std::string label = "position" + std::to_string(i);
                ImGui::DragFloat3(label.c_str(), &spawnData_.positions[i].x, 0.1f);
            }

            if (ImGui::Button("Save")) {
                spawnJson_.Set("maxSpawnCount", spawnData_.maxSpawnCount);

                const int writeCount = (savedSpawnCount_ > spawnData_.maxSpawnCount)
                    ? savedSpawnCount_
                    : spawnData_.maxSpawnCount;

                for (int i = 0; i < writeCount; ++i) {
                    Vector3 p = (i < spawnData_.maxSpawnCount) ? spawnData_.positions[i] : Vector3{};
                    spawnJson_.Set("position" + std::to_string(i), p);
                }
                spawnJson_.Save();
                savedSpawnCount_ = spawnData_.maxSpawnCount;
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        // ボスの近接攻撃
        if (ImGui::TreeNode("MeleeAttack"))
        {
            ImGui::DragFloat("attackColliderSize", &meleeData_.attackColliderSize, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("attackColliderOffset", &meleeData_.attackColliderOffset.x, 0.01f);
            ImGui::DragFloat("attackStartupTime", &meleeData_.attackStartupTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("attackTime", &meleeData_.attackTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("attackRecoveryTime", &meleeData_.attackRecoveryTime, 0.01f, 0.0f, 10.0f);

            if (ImGui::Button("Save"))
            {
                meleeAttackJson_.Set("attackColliderSize", meleeData_.attackColliderSize);
                meleeAttackJson_.Set("attackColliderOffset", meleeData_.attackColliderOffset);
                meleeAttackJson_.Set("attackStartupTime", meleeData_.attackStartupTime);
                meleeAttackJson_.Set("attackTime", meleeData_.attackTime);
                meleeAttackJson_.Set("attackRecoveryTime", meleeData_.attackRecoveryTime);
                meleeAttackJson_.Save();
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        // ボスのジャンプ落下攻撃
        if (ImGui::TreeNode("JumpAttack")) {
            ImGui::DragFloat("airSpeed", &jumpAttackData_.airSpeed, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("jumpVelocityY", &jumpAttackData_.jumpVelocityY, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("jumpAccelerY", &jumpAttackData_.jumpAccelerY, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("attackColliderSize", &jumpAttackData_.attackColliderSize, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("attackColliderOffset", &jumpAttackData_.attackColliderOffset.x, 0.01f);
            ImGui::DragFloat("attackStartupTime", &jumpAttackData_.attackStartupTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("jumpUpTime", &jumpAttackData_.JumpUpTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("airHoldTime", &jumpAttackData_.airHoldTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("fallDownTime", &jumpAttackData_.fallDownTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("attackRecoveryTime", &jumpAttackData_.attackRecoveryTime, 0.01f, 0.0f, 10.0f);

            if (ImGui::Button("Save")) {
                jumpAttackJson_.Set("airSpeed", jumpAttackData_.airSpeed);
                jumpAttackJson_.Set("jumpVelocityY", jumpAttackData_.jumpVelocityY);
                jumpAttackJson_.Set("jumpAccelerY", jumpAttackData_.jumpAccelerY);
                jumpAttackJson_.Set("attackColliderSize", jumpAttackData_.attackColliderSize);
                jumpAttackJson_.Set("attackColliderOffset", jumpAttackData_.attackColliderOffset);
                jumpAttackJson_.Set("attackStartupTime", jumpAttackData_.attackStartupTime);
                jumpAttackJson_.Set("jumpUpTime", jumpAttackData_.JumpUpTime);
                jumpAttackJson_.Set("airHoldTime", jumpAttackData_.airHoldTime);
                jumpAttackJson_.Set("fallDownTime", jumpAttackData_.fallDownTime);
                jumpAttackJson_.Set("attackRecoveryTime", jumpAttackData_.attackRecoveryTime);
                jumpAttackJson_.Save();
            }
            ImGui::TreePop();
        }

        ImGui::Separator();

        // ボスのダッシュ攻撃
        if (ImGui::TreeNode("DashAttack")) {
            ImGui::DragFloat("dashSpeed",&dashAttackData_.dashSpeed, 0.1f, 0.0f, 100.0f);
            ImGui::DragFloat("attackColliderSize",&dashAttackData_.attackColliderSize, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("attackColliderOffset",&dashAttackData_.attackColliderOffset.x, 0.01f);
            ImGui::DragFloat("attackStartupTime",&dashAttackData_.attackStartupTime, 0.01f, 0.0f, 5.0f);
            ImGui::DragFloat("dashTime",&dashAttackData_.dashTime, 0.01f, 0.0f, 5.0f);
            ImGui::DragFloat("attackRecoveryTime",&dashAttackData_.attackRecoveryTime, 0.01f, 0.0f, 5.0f);
            if (ImGui::Button("Save")) {
                dashAttackJson_.Set("dashSpeed", dashAttackData_.dashSpeed);
                dashAttackJson_.Set("attackColliderSize", dashAttackData_.attackColliderSize);
                dashAttackJson_.Set("attackColliderOffset", dashAttackData_.attackColliderOffset);
                dashAttackJson_.Set("attackStartupTime", dashAttackData_.attackStartupTime);
                dashAttackJson_.Set("dashTime", dashAttackData_.dashTime);
                dashAttackJson_.Set("attackRecoveryTime", dashAttackData_.attackRecoveryTime);
                dashAttackJson_.Save();
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
    ImGui::Separator();
    ImGui::End();
}