#pragma once
#include "BLpch.h"
#include "Core.h"

namespace Blanco {
	class BL_API Input {
	public:
		static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
		static bool IsMousePressed(int button) { return s_Instance->IsMousePressedImpl(button); }
		static std::pair<float,float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMousePressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
	private:
		static Input* s_Instance;
	};
}