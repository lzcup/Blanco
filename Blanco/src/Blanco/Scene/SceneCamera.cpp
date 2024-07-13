#include "BLpch.h"
#include "SceneCamera.h"
#include <gtc/matrix_transform.hpp>

namespace Blanco
{
	SceneCamera::SceneCamera()
	{
		ReCalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		BL_CORE_ASSERT(width > 0 && height > 0,"");
		m_AspectRatio = (float)width / (float)height;
		ReCalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float perspectiveNear, float perspectiveFar)
	{
		m_ProjectionType = Perspective;
		m_PerspectiveVerticalFov = verticalFov;
		m_PerspectiveNear = perspectiveNear;
		m_PerspectiveFar = perspectiveFar;
		ReCalculateProjection();
	}

	void SceneCamera::SetOrthographic(float orthographicSize, float orthographicNear, float orthographicFar)
	{
		m_ProjectionType = Orthographic;
		m_OrthographicSize = orthographicSize;
		m_OrthographicNearClip = orthographicNear;
		m_OrthographicFarClip = orthographicFar;
		ReCalculateProjection();
	}

	void SceneCamera::ReCalculateProjection()
	{
		if (m_ProjectionType == Perspective) {
			m_Projection = glm::perspective(m_PerspectiveVerticalFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else {
			float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float right = m_OrthographicSize * m_AspectRatio * 0.5f;
			float bottom = -m_OrthographicSize * 0.5f;
			float top = m_OrthographicSize * 0.5f;
			m_Projection = glm::ortho(left, right, bottom, top, m_OrthographicNearClip, m_OrthographicFarClip);
		}
	}
}

