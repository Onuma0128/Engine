
#pragma once

#include "BaseLight.h"

#include "Vector3.h"
#include "Vector4.h"

/// <summary>
/// 点光源クラス
/// </summary>
class PointLight : public BaseLight
{
public:
    struct PointLightData {
        NumaEngine::Vector4 color;
        NumaEngine::Vector3 position;
        float intensity;
        float radius;
        float decay;
        float padding;
    };

    void Initialize(NumaEngine::DirectXEngine* dxEngine) override;
    void Update() override;
    void Debug_ImGui() override;
    void MakeLightData() override;

    void BuildMatricesCoverAll(
        const NumaEngine::Vector3& sceneMin,
        const NumaEngine::Vector3& sceneMax,
        uint32_t shadowW, uint32_t shadowH) override {}

private:
    PointLightData* lightData_ = nullptr;
};

