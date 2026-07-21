#include "NumbersUI.h"

#include "DirectXEngine.h"

#include <algorithm>
#include <string>

void NumbersUI::Initialize(int maxDigits)
{
    maxDigits_ = std::max(1, maxDigits);

    digitSprites_.clear();
    digitSprites_.reserve(maxDigits_);

    for (int i = 0; i < maxDigits_; ++i)
    {
        auto sprite = std::make_unique<Sprite>();
        sprite->Initialize("numbers.png");
        sprite->SetAnchorPoint({ 0.0f, 0.0f });
        sprite->SetTextureSize({ kSourceDigitWidth, kSourceDigitHeight });
        sprite->GetTransform().size = digitSize_;
        sprite->SetColor(color_);
        sprite->SetSceneRenderer();
        digitSprites_.push_back(std::move(sprite));
    }
}

void NumbersUI::Finalize()
{
    if (DirectXEngine::GetSceneRenderer() != nullptr)
    {
        for (auto& sprite : digitSprites_)
        {
            DirectXEngine::GetSceneRenderer()->SetRemoveList(sprite.get());
        }
    }

    digitSprites_.clear();
    maxDigits_ = 0;
}

void NumbersUI::Update(int number)
{
    if (digitSprites_.empty())
    {
        return;
    }

    number = std::max(0, number);

    std::string text = std::to_string(number);

    if (static_cast<int>(text.size()) > maxDigits_)
    {
        text = text.substr(text.size() - maxDigits_);
    }

    const int visibleDigits = static_cast<int>(text.size());

    // 表示される数字全体の横幅
    const float totalWidth =
        static_cast<float>(visibleDigits) * digitSize_.x +
        static_cast<float>(visibleDigits - 1) * digitSpacing_;

    // position_.xを中心にするための左端座標
    const float startX = position_.x - totalWidth * 0.5f;

    for (int i = 0; i < maxDigits_; ++i)
    {
        Sprite* sprite = digitSprites_[i].get();

        const bool isVisible = i < visibleDigits;
        sprite->GetRenderOptions().enabled = isVisible;

        if (!isVisible)
        {
            continue;
        }

        const int digit = text[i] - '0';

        sprite->SetTextureLeftTop(
            {
                kSourceDigitWidth * static_cast<float>(digit),
                0.0f
            }
        );

        sprite->SetTextureSize(
            {
                kSourceDigitWidth,
                kSourceDigitHeight
            }
        );

        sprite->GetTransform().size = digitSize_;

        sprite->GetTransform().position =
        {
            startX +
                static_cast<float>(i) *
                (digitSize_.x + digitSpacing_),
            position_.y
        };

        sprite->SetColor(color_);
        sprite->Update();
    }
}

void NumbersUI::SetVisible(bool isVisible)
{
    for (auto& sprite : digitSprites_)
    {
        sprite->GetRenderOptions().enabled = isVisible;
        sprite->Update();
    }
}

void NumbersUI::ApplyDigitSprite(Sprite* sprite, int digit, int index)
{
    sprite->SetTextureLeftTop(
        {
            kSourceDigitWidth * static_cast<float>(digit),
            0.0f
        }
    );
    sprite->SetTextureSize({ kSourceDigitWidth, kSourceDigitHeight });

    sprite->GetTransform().size = digitSize_;
    sprite->GetTransform().position =
    {
        position_.x + static_cast<float>(index) * (digitSize_.x + digitSpacing_),
        position_.y
    };

    sprite->SetColor(color_);
}