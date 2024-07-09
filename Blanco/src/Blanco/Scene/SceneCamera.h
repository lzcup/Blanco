#pragma once
#include "Blanco/Renderer/Camera.h"

namespace Blanco
{
	class SceneCamera :public Camera {
	public:
		enum ProjectionType {
			Perspective = 0, Orthographic = 1
		};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; ReCalculateProjection(); }

		void SetViewportSize(uint32_t width, uint32_t height);
		void SetPerspective(float verticalFov, float perspectiveNear, float perspectiveFar);
		void SetOrthographic(float orthographicSize, float orthographicNear, float orthographicFar);

		float GetPerspectiveVerticalFov() const { return m_PerspectiveVerticalFov; }
		void SetPerspectiveVerticalFov(float verticalFov) { m_PerspectiveVerticalFov = verticalFov; }
		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		void SetPerspectiveNear(float perspectiveNear) { m_PerspectiveNear = perspectiveNear; }
		float GetPerspectiveFar() const { return m_PerspectiveFar; }
		void SetPerspectiveFar(float perspectiveFar) { m_PerspectiveFar = perspectiveFar; }


		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float orthographicSize) { m_OrthographicSize = orthographicSize; }
		float GetOrthographicNearClip() const { return m_OrthographicNearClip; }
		void SetOrthographicNearClip(float orthographicNear) { m_OrthographicNearClip = orthographicNear; }
		float GetOrthographicFarClip() const { return m_OrthographicFarClip; }
		void SetOrthographicFarClip(float orthographicFar) { m_OrthographicFarClip = orthographicFar; }

	private:
		void ReCalculateProjection();
	private:
		ProjectionType m_ProjectionType = Orthographic;
		float m_AspectRatio = 16.0f / 9.0f;

		float m_PerspectiveVerticalFov = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 10000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNearClip = -1.0f, m_OrthographicFarClip = 1.0f;
	};
}