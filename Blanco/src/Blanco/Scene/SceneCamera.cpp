#include "BLpch.h"
#include "SceneCamera.h"
#include <gtc/matrix_transform.hpp>

namespace Blanco
{
	SceneCamera::SceneCamera()
	{
		ReCalculateProjection();
	}

	void SceneCamera::SetOrthGraphic(float orthoSize, float orthoNear, float orthoFar)
	{
		m_OrthoGraphicSize = orthoSize;
		m_OrthoGraphicNear = orthoNear;
		m_OrthoGraphicFar = orthoFar;
		ReCalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		ReCalculateProjection();
	}

	void SceneCamera::ReCalculateProjection()
	{
		float left = -m_OrthoGraphicSize * m_AspectRatio * 0.5f;
		float right = m_OrthoGraphicSize * m_AspectRatio * 0.5f;
		float bottom = -m_OrthoGraphicSize * 0.5f;
		float top = m_OrthoGraphicSize * 0.5f;
		m_Projection = glm::ortho(left, right, bottom, top, m_OrthoGraphicNear, m_OrthoGraphicFar);
	}
}

