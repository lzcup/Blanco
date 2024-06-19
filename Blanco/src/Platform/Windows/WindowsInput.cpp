#include "BLpch.h"
#include "WindowsInput.h"
#include "Blanco/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Blanco
{
	Ref<Input> Input::s_Instance = CreateRef<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		BL_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMousePressedImpl(int button)
	{
		BL_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		BL_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos,(float)ypos };
	}
}