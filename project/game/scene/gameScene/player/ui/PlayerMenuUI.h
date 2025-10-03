#pragma once

#include "BaseUI.h"

class PlayerMenuUI : public BaseUI
{
public:

	void Init();

	void Update()override;

	void Draw()override;

private:

	bool isPlay_ = false;

};

