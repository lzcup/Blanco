#pragma once
#include "glm.hpp"

namespace Blanco
{
	class Camera {
	public:
		Camera(const glm::mat4& projection) :m_Projection(projection) {};

		const glm::mat4& GetProjection() const{ return m_Projection; }
	private:
		glm::mat4 m_Projection;
	};


	class OrthoGraphicCamera {
	public:
		OrthoGraphicCamera(float left,float right,float bottom,float top);

		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) { m_Postion = position; ReCalculateProjectionViewMatrix();}
		void SetRotation(float rotation) { m_Rotation = glm::radians(rotation); ReCalculateProjectionViewMatrix();}

		inline const glm::mat4& GetPorjectionMatrix() const{ return m_PorjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const{ return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const{ return m_ViewProjectionMatrix; }

		inline const glm::vec3& GetPostion ()const { return m_Postion; }
		inline float GetRotation() const { return m_Rotation; }
	private:
		void ReCalculateProjectionViewMatrix();
	private:
		glm::mat4 m_PorjectionMatrix, m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Postion;
		float m_Rotation;
	};
}