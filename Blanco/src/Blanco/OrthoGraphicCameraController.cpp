#include "BLpch.h"
#include "OrthoGraphicCameraController.h"
#include "Blanco/Core.h"
#include "Blanco/Input.h"
#include "Blanco/KeyCodes.h"

namespace Blanco
{
	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio), m_ZoomLevel(1.0f), m_Rotation(rotation),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthoGraphicCameraController::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(BL_KEY_W))
			m_CameraPosition.y += m_MoveSpeed * ts;
		else if (Input::IsKeyPressed(BL_KEY_S))
			m_CameraPosition.y -= m_MoveSpeed * ts;

		if (Input::IsKeyPressed(BL_KEY_A))
			m_CameraPosition.x += m_MoveSpeed * ts;
		else if (Input::IsKeyPressed(BL_KEY_D))
			m_CameraPosition.x -= m_MoveSpeed * ts;

		if (m_Rotation) {
			if (Input::IsKeyPressed(BL_KEY_Q))
				m_CameraRotation -= m_RotateSpeed * ts;
			else if (Input::IsKeyPressed(BL_KEY_E))
				m_CameraRotation += m_RotateSpeed * ts;
		}

		m_MoveSpeed = m_ZoomLevel * 5;
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void OrthoGraphicCameraController::OnEvent(Event& e)
	{
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BL_BIND_EVENT_FNC(OrthoGraphicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BL_BIND_EVENT_FNC(OrthoGraphicCameraController::OnWindowResizeEvent));
	}

	bool OrthoGraphicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(0.25f, m_ZoomLevel);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthoGraphicCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}

