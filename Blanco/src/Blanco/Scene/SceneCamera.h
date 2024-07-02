#pragma once
#include "Blanco/Renderer/Camera.h"

namespace Blanco
{
	class SceneCamera :public Camera {
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthGraphic(float orthoSize, float orthoNear, float orthoFar);
		void SetOrthGraphicSize(float orthoSize) { m_OrthoGraphicSize = orthoSize; ReCalculateProjection(); }
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthoGraphicSize() const { return m_OrthoGraphicSize; }
	private:
		void ReCalculateProjection();
	private:
		float m_OrthoGraphicSize = 10.0f;
		float m_OrthoGraphicNear = -1.0f, m_OrthoGraphicFar = 1.0f;
		float m_AspectRatio = 16.0f / 9.0f;
	};
}