#pragma once
#include <string>
#include <glm.hpp>

namespace Blanco
{
	class Shader {
	public:
		Shader(std::string vertexSrc, std::string fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& value);
	private:
		uint32_t m_RendererID;
	};
}