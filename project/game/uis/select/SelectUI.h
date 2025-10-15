#pragma once

#include "BaseUI.h"

class SelectUI : public BaseUI
{
public:

	void Init(const std::string& name);

	void Update()override;

	void Draw()override;

private:

};

