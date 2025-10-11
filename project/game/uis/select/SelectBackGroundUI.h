#pragma once

#include "BaseUI.h"

class SelectBackGroundUI : public BaseUI
{
public:

	void Init(const std::string& name);

	void Update()override;

	void Draw()override;

private:

};

