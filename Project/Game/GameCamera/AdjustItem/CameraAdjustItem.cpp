#include "CameraAdjustItem.h"

#include "imgui.h"

#include "CameraManager.h"

void CameraAdjustItem::LoadItems() 
{
    /* ============================== GameCamera ============================== */
    cameraJson_.Init("GameCamera");
    if (!cameraJson_.Load()) {
        cameraJson_.Set("mainRotate", Vector3{});
        cameraJson_.Set("mainPosition", Vector3{});

        cameraData_.maxClearCameraPoint = 0;
        cameraJson_.Set("maxClearCameraPoint", 0);
        cameraJson_.Set("clearTime0", 0.0f);
        cameraJson_.Set("clearRotate0", Vector3{});
        cameraJson_.Set("clearPosition0", Vector3{});
        cameraJson_.Set("clearShackIndex", 0);
        cameraJson_.Set("clearShackPow", 0.0f);

        cameraJson_.Set("sabPosition", Vector3{});
        cameraJson_.Set("isSabRotate", false);
        cameraJson_.Set("sabRotateSpeed", 0.0f);
        cameraJson_.Set("sabRadius", 0.0f);
        cameraJson_.Set("sabPosY", 0.0f);

        cameraJson_.Set("bossStartupTime", 0.0f);
        cameraJson_.Set("bossActiveTime", 0.0f);
        cameraJson_.Set("bossRecoverTime", 0.0f);
        cameraJson_.Set("bossStartRotate", Vector3{});
        cameraJson_.Set("bossStartPosition", Vector3{});
        cameraJson_.Set("bossEndRotate", Vector3{});
        cameraJson_.Set("bossEndPosition", Vector3{});

    } else {
        cameraData_.mainPosition = cameraJson_.Get("mainPosition", cameraData_.mainPosition);
        cameraData_.mainRotate = cameraJson_.Get("mainRotate", cameraData_.mainRotate);

        cameraData_.maxClearCameraPoint = cameraJson_.Get("maxClearCameraPoint", cameraData_.maxClearCameraPoint);
        cameraData_.clearData.resize(cameraData_.maxClearCameraPoint);
        for (int i = 0; i < cameraData_.maxClearCameraPoint; ++i) {
            cameraData_.clearData[i].time = cameraJson_.Get("clearTime" + std::to_string(i), 0.0f);
            cameraData_.clearData[i].rotate = cameraJson_.Get("clearRotate" + std::to_string(i), Vector3{});
            cameraData_.clearData[i].position = cameraJson_.Get("clearPosition" + std::to_string(i), Vector3{});
        }
        cameraData_.clearShackIndex = cameraJson_.Get("clearShackIndex", cameraData_.clearShackIndex);
        cameraData_.clearShackPow = cameraJson_.Get("clearShackPow", cameraData_.clearShackPow);

        cameraData_.sabPosition = cameraJson_.Get("sabPosition", cameraData_.sabPosition);
        cameraData_.isSabRotate = cameraJson_.Get("isSabRotate", cameraData_.isSabRotate);
        cameraData_.sabRotateSpeed = cameraJson_.Get("sabRotateSpeed", cameraData_.sabRotateSpeed);
        cameraData_.sabRadius = cameraJson_.Get("sabRadius", cameraData_.sabRadius);
        cameraData_.sabPosY = cameraJson_.Get("sabPosY", cameraData_.sabPosY);

        cameraData_.bossStartupTime = cameraJson_.Get("bossStartupTime", cameraData_.bossStartupTime);
        cameraData_.bossActiveTime = cameraJson_.Get("bossActiveTime", cameraData_.bossActiveTime);
        cameraData_.bossRecoverTime = cameraJson_.Get("bossRecoverTime", cameraData_.bossRecoverTime);
        cameraData_.bossStartRotate = cameraJson_.Get("bossStartRotate", cameraData_.bossStartRotate);
        cameraData_.bossStartPosition = cameraJson_.Get("bossStartPosition", cameraData_.bossStartPosition);
        cameraData_.bossEndRotate = cameraJson_.Get("bossEndRotate", cameraData_.bossEndRotate);
        cameraData_.bossEndPosition = cameraJson_.Get("bossEndPosition", cameraData_.bossEndPosition);
    }
    savedSpawnCount_ = cameraData_.maxClearCameraPoint;
}

void CameraAdjustItem::Editor()
{
    ImGui::Begin("GameData");

    if (ImGui::TreeNode("GameCamera"))
    {
        ImGui::PushItemWidth(220.0f);
        ImGui::SeparatorText("Active Camera");

        if (ImGui::Button("mainCamera")) { CameraManager::GetInstance()->SetActiveCamera(0); }
        ImGui::SameLine();
        if (ImGui::Button("sabCamera")) { CameraManager::GetInstance()->SetActiveCamera(1); }
        ImGui::SameLine();
        if (ImGui::Button("bossCamera")) { CameraManager::GetInstance()->SetActiveCamera(2); }

        ImGui::Spacing();
        ImGui::SeparatorText("Main Camera");

        ImGui::DragFloat3("mainPosition", &cameraData_.mainPosition.x, 0.05f);
        ImGui::DragFloat3("mainRotate", &cameraData_.mainRotate.x, 0.05f);

        ImGui::Spacing();
        ImGui::SeparatorText("Clear Camera");

        ImGui::DragInt("clearShackIndex", &cameraData_.clearShackIndex, 1, 0, cameraData_.maxClearCameraPoint - 1);
        ImGui::DragFloat("clearShackPow", &cameraData_.clearShackPow, 0.01f);

        ImGui::Text("maxPoint: %d", cameraData_.maxClearCameraPoint);
        ImGui::SameLine();
        if (ImGui::Button("-")) {
            if (cameraData_.maxClearCameraPoint > 0) {
                cameraData_.maxClearCameraPoint--;
                if ((int)cameraData_.clearData.size() > cameraData_.maxClearCameraPoint) {
                    cameraData_.clearData.resize(cameraData_.maxClearCameraPoint);
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("+")) {
            cameraData_.maxClearCameraPoint++;
            if ((int)cameraData_.clearData.size() < cameraData_.maxClearCameraPoint) {
                cameraData_.clearData.resize(cameraData_.maxClearCameraPoint, ClearCameraData{});
            }
        }

        ImGui::Separator();

        for (int i = 0; i < cameraData_.maxClearCameraPoint; ++i) {
            std::string time = "clearTime" + std::to_string(i);
            std::string rotate = "clearRotate" + std::to_string(i);
            std::string pos = "clearPosition" + std::to_string(i);
            ImGui::DragFloat(time.c_str(), &cameraData_.clearData[i].time, 0.1f);
            ImGui::DragFloat3(rotate.c_str(), &cameraData_.clearData[i].rotate.x, 0.1f);
            ImGui::DragFloat3(pos.c_str(), &cameraData_.clearData[i].position.x, 0.1f);
            ImGui::Separator();
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Sab Camera");

        ImGui::DragFloat3("sabPosition", &cameraData_.sabPosition.x, 0.05f);
        ImGui::Checkbox("isSabRotate", &cameraData_.isSabRotate);
        ImGui::DragFloat("sabRotateSpeed", &cameraData_.sabRotateSpeed, 0.01f, -100.0f, 100.0f, "%.3f");
        ImGui::DragFloat("sabRadius", &cameraData_.sabRadius, 0.01f, 0.0f, 10000.0f, "%.3f");
        ImGui::DragFloat("sabPosY", &cameraData_.sabPosY, 0.01f, -10000.0f, 10000.0f, "%.3f");

        ImGui::Spacing();
        ImGui::SeparatorText("Boss Camera");

        ImGui::DragFloat("bossStartupTime", &cameraData_.bossStartupTime, 0.01f, 0.0f, 1000.0f, "%.3f");
        ImGui::DragFloat("bossActiveTime", &cameraData_.bossActiveTime, 0.01f, 0.0f, 1000.0f, "%.3f");
        ImGui::DragFloat("bossRecoverTime", &cameraData_.bossRecoverTime, 0.01f, 0.0f, 1000.0f, "%.3f");
        ImGui::DragFloat3("bossStartRotate", &cameraData_.bossStartRotate.x, 0.05f);
        ImGui::DragFloat3("bossStartPosition", &cameraData_.bossStartPosition.x, 0.05f);
        ImGui::DragFloat3("bossEndRotate", &cameraData_.bossEndRotate.x, 0.05f);
        ImGui::DragFloat3("bossEndPosition", &cameraData_.bossEndPosition.x, 0.05f);

        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::Button("Save"))
        {
            // cameraData_ -> cameraJson_ に書き戻してから保存
            cameraJson_.Set("mainRotate", cameraData_.mainRotate);
            cameraJson_.Set("mainPosition", cameraData_.mainPosition);

            cameraJson_.Set("maxClearCameraPoint", cameraData_.maxClearCameraPoint);
            const int writeCount = (savedSpawnCount_ > cameraData_.maxClearCameraPoint)
                ? savedSpawnCount_
                : cameraData_.maxClearCameraPoint;

            for (int i = 0; i < writeCount; ++i) {
                float time = (i < cameraData_.maxClearCameraPoint) ? cameraData_.clearData[i].time : 0.0f;
                Vector3 rotate = (i < cameraData_.maxClearCameraPoint) ? cameraData_.clearData[i].rotate : Vector3{};
                Vector3 pos = (i < cameraData_.maxClearCameraPoint) ? cameraData_.clearData[i].position : Vector3{};
                cameraJson_.Set("clearTime" + std::to_string(i), time);
                cameraJson_.Set("clearRotate" + std::to_string(i), rotate);
                cameraJson_.Set("clearPosition" + std::to_string(i), pos);
            }
            savedSpawnCount_ = cameraData_.maxClearCameraPoint;
            cameraJson_.Set("clearShackIndex", cameraData_.clearShackIndex);
            cameraJson_.Set("clearShackPow", cameraData_.clearShackPow);

            cameraJson_.Set("sabPosition", cameraData_.sabPosition);
            cameraJson_.Set("isSabRotate", cameraData_.isSabRotate);
            cameraJson_.Set("sabRotateSpeed", cameraData_.sabRotateSpeed);
            cameraJson_.Set("sabRadius", cameraData_.sabRadius);
            cameraJson_.Set("sabPosY", cameraData_.sabPosY);

            cameraJson_.Set("bossStartupTime", cameraData_.bossStartupTime);
            cameraJson_.Set("bossActiveTime", cameraData_.bossActiveTime);
            cameraJson_.Set("bossRecoverTime", cameraData_.bossRecoverTime);
            cameraJson_.Set("bossStartRotate", cameraData_.bossStartRotate);
            cameraJson_.Set("bossStartPosition", cameraData_.bossStartPosition);
            cameraJson_.Set("bossEndRotate", cameraData_.bossEndRotate);
            cameraJson_.Set("bossEndPosition", cameraData_.bossEndPosition);

            cameraJson_.Save();
        }
        ImGui::SameLine();

        ImGui::PopItemWidth();
        ImGui::TreePop();
    }
    ImGui::Separator();

    ImGui::End();
}
