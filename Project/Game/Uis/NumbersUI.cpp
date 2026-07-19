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
        ApplyDigitSprite(sprite, digit, i);
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