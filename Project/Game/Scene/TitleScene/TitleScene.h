#pragma once

#include <memory>

#include "BaseScene.h"
#include "BaseUI.h"

class TitleScene : public BaseScene
{
public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:

    // フェード
    std::unique_ptr<BaseUI> sceneFade_ = nullptr;
    bool isFade_ = false;

};

