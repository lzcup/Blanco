#include "BLpch.h"
#include "OpenGLShader.h"
#include "glad/gl.h"
#include "gtc/type_ptr.hpp"
#include <fstream>

namespace Blanco
{
	static GLenum ShaderTypeToGLenum(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		BL_CORE_ASSERT(false, "Unknow shader type!")
			return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		BL_PROFILE_FUNCTION();

		size_t begin = filepath.find_last_of("/\\") == std::string::npos ? 0 : filepath.find_last_of("/\\") + 1;
		size_t end = filepath.rfind(".");
		size_t counts = end == std::string::npos ? filepath.size() - begin : end - begin;
		m_Name = filepath.substr(begin, counts);

		std::string shaderSource = ReadFile(filepath);
		auto shaderMap = PreProcess(shaderSource);
		Compile(shaderMap);
	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
		m_Name(name)
	{
		BL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderMap;
		shaderMap[GL_VERTEX_SHADER] = vertexSrc;
		shaderMap[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderMap);
	}

	OpenGLShader::~OpenGLShader()
	{
		BL_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform1iv(location, count, values);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform1f(location, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			BL_CORE_WARN("Uniform(" + name + ")is not exist!");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		BL_PROFILE_FUNCTION();

		std::ifstream in(filepath, std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			size_t length = in.tellg();
			in.seekg(0, std::ios::beg);
			char* buffer = new char[length + 1];
			buffer[length] = '\0';
			in.read(buffer, length);
			in.close();
			return buffer;
		}
		BL_CORE_ASSERT(false, "Can't read file:" + filepath)
			return nullptr;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		BL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderMap;
		std::string token = "#type";
		size_t pos = source.find(token);
		size_t tokenLength = token.length();
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			BL_CORE_ASSERT((eol != std::string::npos), "Syntax error!")
				std::string type = source.substr(pos + tokenLength + 1, eol - (pos + tokenLength + 1));
			GLenum glType = ShaderTypeToGLenum(type);
			size_t nextLine = source.find_first_not_of("\r\n", eol);
			pos = source.find("#type", eol);
			std::string value = source.substr(nextLine, pos - nextLine);
			shaderMap[glType] = value;
		}
		return shaderMap;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderMap)
	{
		BL_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		BL_CORE_ASSERT(shaderMap.size() <= 2, "Not support more than 2 shaders for now!")
			std::array<GLuint, 2> shaderIDs = { 0,0 };
		int index = 0;
		for (auto& kv : shaderMap) {
			GLenum shadeType = kv.first;
			const std::string& value = kv.second;

			GLuint shader = glCreateShader(shadeType);

			const char* source = value.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				BL_CORE_ERROR(infoLog.data());
				BL_CORE_ASSERT(false, "Shader compile failed!")
			}
			glAttachShader(program, shader);
			shaderIDs[index++] = shader;
		}

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
			for (auto& shader : shaderIDs)
				glDeleteShader(shader);

			BL_CORE_ERROR(infoLog.data());
			BL_CORE_ASSERT(false, "Program link failed!")
		}

		// Always detach shaders after a successful link.
		for (auto& shader : shaderIDs)
			glDetachShader(program, shader);

		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		BL_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		BL_PROFILE_FUNCTION();

		glUseProgram(0);
	}
	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		BL_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		BL_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& values)
	{
		BL_PROFILE_FUNCTION();

		UploadUniformMat4(name, values);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		BL_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		UploadUniformFloat2(name, values);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		BL_PROFILE_FUNCTION();

		UploadUniformFloat3(name, values);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		BL_PROFILE_FUNCTION();

		UploadUniformFloat4(name, values);
	}
}