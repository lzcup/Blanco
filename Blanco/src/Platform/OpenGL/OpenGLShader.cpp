#include "BLpch.h"
#include "OpenGLShader.h"
#include "glad/gl.h"
#include "gtc/type_ptr.hpp"

namespace Blanco
{
	OpenGLShader::OpenGLShader(std::string vertexSrc, std::string fragmentSrc)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			BL_CORE_ERROR(infoLog.data());
			BL_CORE_ASSERT(false, "VertextShader compile failed!")
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			BL_CORE_ERROR(infoLog.data());
			BL_CORE_ASSERT(false, "FragmentShader compile failed!")
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			BL_CORE_ERROR(infoLog.data());
			BL_CORE_ASSERT(false, "Program link failed!")
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform1f(location, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}
}
