#include "BLpch.h"
#include "Camera.h"
#include "RendererAPI.h"
#include "gtc/matrix_transform.hpp"

namespace Blanco
{
	OrthoGraphicCamera::OrthoGraphicCamera(float left, float right, float bottom, float top) :
		m_ViewMatrix(1.0f), m_Postion(0.0f), m_Rotation(0.0f)
	{
		BL_PROFILE_FUNCTION();

		m_PorjectionMatrix = glm::ortho(left, right, bottom, top, -0.5f, 0.5f);
		m_ViewProjectionMatrix = m_PorjectionMatrix * m_ViewMatrix;
	}
	void OrthoGraphicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		BL_PROFILE_FUNCTION();

		m_PorjectionMatrix = glm::ortho(left, right, bottom, top, -0.5f, 0.5f);
		m_ViewProjectionMatrix = m_PorjectionMatrix * m_ViewMatrix;
	}
	void OrthoGraphicCamera::ReCalculateProjectionViewMatrix()
	{
		BL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Postion) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_PorjectionMatrix * m_ViewMatrix;
	}
}
