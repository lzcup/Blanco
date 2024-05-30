#pragma once
#include "Blanco/Input.h"

namespace Blanco 
{
	class WindowsInput :public Input {
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) override;
		virtual bool IsMousePressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
	};
}