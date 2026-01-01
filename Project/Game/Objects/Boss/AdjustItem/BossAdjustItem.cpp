#include "BossAdjustItem.h"

#include "imgui.h"

void BossAdjustItem::LoadItems()
{
	/* ============================== Main ============================== */
	mainJson_.Init("BossMain");
	if (!mainJson_.Load()) {
        mainJson_.Set("startPosition", Vector3{});
		mainJson_.Set("maxHP", 20);
		mainJson_.Set("speed", 2.0f);
        mainJson_.Set("rayDistance", 2.0f);
        mainJson_.Set("rayOffset", Vector3{});
        mainJson_.Set("colliderSize", 0.0f);
        mainJson_.Set("colliderOffset", Vector3{});
		mainJson_.Set("searchUpdateTime", 0.0f);
	} else {
        mainData_.startPosition = mainJson_.Get("startPosition", mainData_.startPosition);
		mainData_.maxHP = mainJson_.Get("maxHP", mainData_.maxHP);
		mainData_.speed = mainJson_.Get("speed", mainData_.speed);
        mainData_.rayDistance = mainJson_.Get("rayDistance", mainData_.rayDistance);
        mainData_.rayOffset = mainJson_.Get("rayOffset", mainData_.rayOffset);
        mainData_.colliderSize = mainJson_.Get("colliderSize", mainData_.colliderSize);
        mainData_.colliderOffset = mainJson_.Get("colliderOffset", mainData_.colliderOffset);
		mainData_.searchUpdateTime = mainJson_.Get("searchUpdateTime", mainData_.searchUpdateTime);
	}
	mainData_.debugSpline = false;

    /* ============================== Down ============================== */
    downJson_.Init("BossDown");
    if (!downJson_.Load()) {
        downJson_.Set("downTime", 0.0f);
    } else {
        downData_.downTime = downJson_.Get("downTime", downData_.downTime);
        
    }

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
        meleeAttackJson_.Set("jumpVelocityY", 0.0f);
        meleeAttackJson_.Set("jumpAccelerY", 0.0f);
        meleeAttackJson_.Set("attackColliderSize", meleeData_.attackColliderSize);
        meleeAttackJson_.Set("attackColliderOffset", meleeData_.attackColliderOffset);
        meleeAttackJson_.Set("attackStartupTime", meleeData_.attackStartupTime);
        meleeAttackJson_.Set("attackTime", meleeData_.attackTime);
        meleeAttackJson_.Set("attackRecoveryTime", meleeData_.attackRecoveryTime);
    } else
    {
        meleeData_.jumpVelocityY = meleeAttackJson_.Get("jumpVelocityY", meleeData_.jumpVelocityY);
        meleeData_.jumpAccelerY = meleeAttackJson_.Get("jumpAccelerY", meleeData_.jumpAccelerY);
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

    // ============================== BossStateScore ==============================
    stateScoreJson_.Init("BossStateScore");

    if (!stateScoreJson_.Load())
    {
        stateScoreJson_.Set("scoreAccumulationTime", stateScoreData_.scoreAccumulationTime);

        stateScoreJson_.Set("phaseHpThreshold", stateScoreData_.phaseHpThreshold);
        stateScoreJson_.Set("playerToDistance", stateScoreData_.playerToDistance);

        stateScoreJson_.Set("spawnHpHigh", stateScoreData_.spawnHpHigh);
        stateScoreJson_.Set("spawnHpLow", stateScoreData_.spawnHpLow);
        stateScoreJson_.Set("spawnDistanceHigh", stateScoreData_.spawnDistanceHigh);
        stateScoreJson_.Set("spawnDistanceLow", stateScoreData_.spawnDistanceLow);
        stateScoreJson_.Set("spawnLookHigh", stateScoreData_.spawnLookHigh);
        stateScoreJson_.Set("spawnLookLow", stateScoreData_.spawnLookLow);

        stateScoreJson_.Set("jumpAttackHpHigh", stateScoreData_.jumpAttackHpHigh);
        stateScoreJson_.Set("jumpAttackHpLow", stateScoreData_.jumpAttackHpLow);
        stateScoreJson_.Set("jumpAttackDistanceHigh", stateScoreData_.jumpAttackDistanceHigh);
        stateScoreJson_.Set("jumpAttackDistanceLow", stateScoreData_.jumpAttackDistanceLow);
        stateScoreJson_.Set("jumpAttackLookHigh", stateScoreData_.jumpAttackLookHigh);
        stateScoreJson_.Set("jumpAttackLookLow", stateScoreData_.jumpAttackLookLow);

        stateScoreJson_.Set("dashAttackHpHigh", stateScoreData_.dashAttackHpHigh);
        stateScoreJson_.Set("dashAttackHpLow", stateScoreData_.dashAttackHpLow);
        stateScoreJson_.Set("dashAttackDistanceHigh", stateScoreData_.dashAttackDistanceHigh);
        stateScoreJson_.Set("dashAttackDistanceLow", stateScoreData_.dashAttackDistanceLow);
        stateScoreJson_.Set("dashAttackLookHigh", stateScoreData_.dashAttackLookHigh);
        stateScoreJson_.Set("dashAttackLookLow", stateScoreData_.dashAttackLookLow);
    } else
    {
        // ---- 読み込み ----
        stateScoreData_.scoreAccumulationTime = stateScoreJson_.Get("scoreAccumulationTime", stateScoreData_.scoreAccumulationTime);

        stateScoreData_.phaseHpThreshold = stateScoreJson_.Get("phaseHpThreshold", stateScoreData_.phaseHpThreshold);
        stateScoreData_.playerToDistance = stateScoreJson_.Get("playerToDistance", stateScoreData_.playerToDistance);

        stateScoreData_.spawnHpHigh = stateScoreJson_.Get("spawnHpHigh", stateScoreData_.spawnHpHigh);
        stateScoreData_.spawnHpLow = stateScoreJson_.Get("spawnHpLow", stateScoreData_.spawnHpLow);
        stateScoreData_.spawnDistanceHigh = stateScoreJson_.Get("spawnDistanceHigh", stateScoreData_.spawnDistanceHigh);
        stateScoreData_.spawnDistanceLow = stateScoreJson_.Get("spawnDistanceLow", stateScoreData_.spawnDistanceLow);
        stateScoreData_.spawnLookHigh = stateScoreJson_.Get("spawnLookHigh", stateScoreData_.spawnLookHigh);
        stateScoreData_.spawnLookLow = stateScoreJson_.Get("spawnLookLow", stateScoreData_.spawnLookLow);

        stateScoreData_.jumpAttackHpHigh = stateScoreJson_.Get("jumpAttackHpHigh", stateScoreData_.jumpAttackHpHigh);
        stateScoreData_.jumpAttackHpLow = stateScoreJson_.Get("jumpAttackHpLow", stateScoreData_.jumpAttackHpLow);
        stateScoreData_.jumpAttackDistanceHigh = stateScoreJson_.Get("jumpAttackDistanceHigh", stateScoreData_.jumpAttackDistanceHigh);
        stateScoreData_.jumpAttackDistanceLow = stateScoreJson_.Get("jumpAttackDistanceLow", stateScoreData_.jumpAttackDistanceLow);
        stateScoreData_.jumpAttackLookHigh = stateScoreJson_.Get("jumpAttackLookHigh", stateScoreData_.jumpAttackLookHigh);
        stateScoreData_.jumpAttackLookLow = stateScoreJson_.Get("jumpAttackLookLow", stateScoreData_.jumpAttackLookLow);

        stateScoreData_.dashAttackHpHigh = stateScoreJson_.Get("dashAttackHpHigh", stateScoreData_.dashAttackHpHigh);
        stateScoreData_.dashAttackHpLow = stateScoreJson_.Get("dashAttackHpLow", stateScoreData_.dashAttackHpLow);
        stateScoreData_.dashAttackDistanceHigh = stateScoreJson_.Get("dashAttackDistanceHigh", stateScoreData_.dashAttackDistanceHigh);
        stateScoreData_.dashAttackDistanceLow = stateScoreJson_.Get("dashAttackDistanceLow", stateScoreData_.dashAttackDistanceLow);
        stateScoreData_.dashAttackLookHigh = stateScoreJson_.Get("dashAttackLookHigh", stateScoreData_.dashAttackLookHigh);
        stateScoreData_.dashAttackLookLow = stateScoreJson_.Get("dashAttackLookLow", stateScoreData_.dashAttackLookLow);
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
            ImGui::DragFloat3("startPosition", &mainData_.startPosition.x, 0.01f, 0.0f, 100.0f);
            ImGui::DragInt("maxHP", &mainData_.maxHP, 1, 1, 1000);
            ImGui::DragFloat("speed", &mainData_.speed, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("rayDistance", &mainData_.rayDistance, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("rayOffset", &mainData_.rayOffset.x, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("colliderSize", &mainData_.colliderSize, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("colliderOffset", &mainData_.colliderOffset.x, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat("searchUpdateTime", &mainData_.searchUpdateTime, 0.01f, 0.0f, 100.0f);
            ImGui::Checkbox("debugSpline", &mainData_.debugSpline);

            if (ImGui::Button("Save")) {
                mainJson_.Set("startPosition", mainData_.startPosition);
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

        // ダウン項目
        if (ImGui::TreeNode("Down")) {
            ImGui::DragFloat("downTime", &downData_.downTime, 0.01f, 0.0f, 100.0f);

            if (ImGui::Button("Save")) {
                downJson_.Set("downTime", downData_.downTime);
                downJson_.Save();
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
            ImGui::DragFloat("jumpVelocityY", &meleeData_.jumpVelocityY, 0.01f, -100.0f, 100.0f);
            ImGui::DragFloat("jumpAccelerY", &meleeData_.jumpAccelerY, 0.01f, -100.0f, 100.0f);

            ImGui::DragFloat("attackColliderSize", &meleeData_.attackColliderSize, 0.01f, 0.0f, 100.0f);
            ImGui::DragFloat3("attackColliderOffset", &meleeData_.attackColliderOffset.x, 0.01f);
            ImGui::DragFloat("attackStartupTime", &meleeData_.attackStartupTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("attackTime", &meleeData_.attackTime, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("attackRecoveryTime", &meleeData_.attackRecoveryTime, 0.01f, 0.0f, 10.0f);

            if (ImGui::Button("Save"))
            {
                meleeAttackJson_.Set("jumpVelocityY", meleeData_.jumpVelocityY);
                meleeAttackJson_.Set("jumpAccelerY", meleeData_.jumpAccelerY);
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

        ImGui::Separator();

        // ステートのスコア計算
        if (ImGui::TreeNode("StateScore"))
        {
            ImGui::DragFloat("scoreAccumulationTime", &stateScoreData_.scoreAccumulationTime, 0.01f, 0.0f, 10.0f);

            ImGui::Separator();
            ImGui::DragInt("phaseHpThreshold", &stateScoreData_.phaseHpThreshold, 1, 0, 9999);
            ImGui::DragFloat("playerToDistance", &stateScoreData_.playerToDistance, 0.01f, 0.0f, 9999.0f);

            ImGui::SeparatorText("Spawn Score");
            ImGui::DragInt("spawnHpHigh", &stateScoreData_.spawnHpHigh);
            ImGui::DragInt("spawnHpLow", &stateScoreData_.spawnHpLow);
            ImGui::DragInt("spawnDistanceHigh", &stateScoreData_.spawnDistanceHigh);
            ImGui::DragInt("spawnDistanceLow", &stateScoreData_.spawnDistanceLow);
            ImGui::DragInt("spawnLookHigh", &stateScoreData_.spawnLookHigh);
            ImGui::DragInt("spawnLookLow", &stateScoreData_.spawnLookLow);

            ImGui::SeparatorText("JumpAttack Score");
            ImGui::DragInt("jumpAttackHpHigh", &stateScoreData_.jumpAttackHpHigh);
            ImGui::DragInt("jumpAttackHpLow", &stateScoreData_.jumpAttackHpLow);
            ImGui::DragInt("jumpAttackDistanceHigh", &stateScoreData_.jumpAttackDistanceHigh);
            ImGui::DragInt("jumpAttackDistanceLow", &stateScoreData_.jumpAttackDistanceLow);
            ImGui::DragInt("jumpAttackLookHigh", &stateScoreData_.jumpAttackLookHigh);
            ImGui::DragInt("jumpAttackLookLow", &stateScoreData_.jumpAttackLookLow);

            ImGui::SeparatorText("DashAttack Score");
            ImGui::DragInt("dashAttackHpHigh", &stateScoreData_.dashAttackHpHigh);
            ImGui::DragInt("dashAttackHpLow", &stateScoreData_.dashAttackHpLow);
            ImGui::DragInt("dashAttackDistanceHigh", &stateScoreData_.dashAttackDistanceHigh);
            ImGui::DragInt("dashAttackDistanceLow", &stateScoreData_.dashAttackDistanceLow);
            ImGui::DragInt("dashAttackLookHigh", &stateScoreData_.dashAttackLookHigh);
            ImGui::DragInt("dashAttackLookLow", &stateScoreData_.dashAttackLookLow);

            if (ImGui::Button("Save##BossStateScore"))
            {
                stateScoreJson_.Set("scoreAccumulationTime", stateScoreData_.scoreAccumulationTime);

                stateScoreJson_.Set("phaseHpThreshold", stateScoreData_.phaseHpThreshold);
                stateScoreJson_.Set("playerToDistance", stateScoreData_.playerToDistance);

                stateScoreJson_.Set("spawnHpHigh", stateScoreData_.spawnHpHigh);
                stateScoreJson_.Set("spawnHpLow", stateScoreData_.spawnHpLow);
                stateScoreJson_.Set("spawnDistanceHigh", stateScoreData_.spawnDistanceHigh);
                stateScoreJson_.Set("spawnDistanceLow", stateScoreData_.spawnDistanceLow);
                stateScoreJson_.Set("spawnLookHigh", stateScoreData_.spawnLookHigh);
                stateScoreJson_.Set("spawnLookLow", stateScoreData_.spawnLookLow);

                stateScoreJson_.Set("jumpAttackHpHigh", stateScoreData_.jumpAttackHpHigh);
                stateScoreJson_.Set("jumpAttackHpLow", stateScoreData_.jumpAttackHpLow);
                stateScoreJson_.Set("jumpAttackDistanceHigh", stateScoreData_.jumpAttackDistanceHigh);
                stateScoreJson_.Set("jumpAttackDistanceLow", stateScoreData_.jumpAttackDistanceLow);
                stateScoreJson_.Set("jumpAttackLookHigh", stateScoreData_.jumpAttackLookHigh);
                stateScoreJson_.Set("jumpAttackLookLow", stateScoreData_.jumpAttackLookLow);

                stateScoreJson_.Set("dashAttackHpHigh", stateScoreData_.dashAttackHpHigh);
                stateScoreJson_.Set("dashAttackHpLow", stateScoreData_.dashAttackHpLow);
                stateScoreJson_.Set("dashAttackDistanceHigh", stateScoreData_.dashAttackDistanceHigh);
                stateScoreJson_.Set("dashAttackDistanceLow", stateScoreData_.dashAttackDistanceLow);
                stateScoreJson_.Set("dashAttackLookHigh", stateScoreData_.dashAttackLookHigh);
                stateScoreJson_.Set("dashAttackLookLow", stateScoreData_.dashAttackLookLow);

                stateScoreJson_.Save();
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
    ImGui::Separator();
    ImGui::End();
}