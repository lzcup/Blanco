#pragma once
#include "BLpch.h"
#include "Core.h"

namespace Blanco {
	class Input {
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsMousePressed(int button);
		static std::pair<float, float> GetMousePos();
	};
}