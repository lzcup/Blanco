#pragma once
#include "Blanco/Renderer/Camera.h"
#include "Blanco/Core//TimeStep.h"
#include "Blanco/Events/MouseEvent.h"
#include "Blanco/Events/ApplicationEvent.h"

namespace Blanco
{
	class OrthoGraphicCameraController {
	public:
		OrthoGraphicCameraController(float aspectRatio, bool rotation = false);
		~OrthoGraphicCameraController() {};

		OrthoGraphicCamera& GetCamera() { return m_Camera; }
		const OrthoGraphicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		bool m_Rotation;
		OrthoGraphicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_MoveSpeed = 5.0f;
		float m_CameraRotation = 0.0f;
		float m_RotateSpeed = 180.0f;
	};
}