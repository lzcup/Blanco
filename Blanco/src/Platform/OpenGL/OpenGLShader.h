#pragma once
#include <string>
#include <glm.hpp>
#include "Blanco/Renderer/Shader.h"

namespace Blanco
{
	//TODO:remove
	typedef unsigned int GLenum;

	class OpenGLShader: public Shader{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetMat4(const std::string& name, const glm::mat4& values) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, int value) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& value) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& value) const;
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum,std::string> PreProcess(const std::string& shader);
		void Compile(const std::unordered_map<GLenum, std::string>& map);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}