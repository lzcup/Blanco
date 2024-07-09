#include "BLpch.h"
#include "Blanco/Core/Input.h"
#include "Blanco/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Blanco
{
	bool Input::IsKeyPressed(int keyCode)
	{
		BL_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMousePressed(int button)
	{
		BL_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	std::pair<float, float> Input::GetMousePos()
	{
		BL_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos,(float)ypos };
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePos();
		return x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePos();
		return y;
	}
}