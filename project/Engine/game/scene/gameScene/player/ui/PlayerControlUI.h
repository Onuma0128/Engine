#pragma once

#include "BaseUI.h"

class PlayerControlUI : public BaseUI
{
public:

	void Init();

	void Update()override;

	void Draw()override;

private:

	bool isPlay_ = false;

};

