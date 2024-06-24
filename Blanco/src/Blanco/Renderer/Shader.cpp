#include "BLpch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Blanco
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLShader>(filepath);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader)
	{
		BL_CORE_ASSERT(!Exist(name), "shader(" + name + ")already exist")
		m_ShaderMap[name] = shader;
	}

	void ShaderLibrary::Add(Ref<Shader> shader)
	{
		const std::string& name = shader->GetName();
		BL_CORE_ASSERT(!Exist(name), "shader(" + name + ") already exist")
		m_ShaderMap[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		Ref<Shader> shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		BL_CORE_ASSERT(Exist(name), "shader(" + name + ") not exist")
		return m_ShaderMap[name];
	}

	bool ShaderLibrary::Exist(const std::string& name)
	{
		if (m_ShaderMap[name])
			return true;
		return false;
	}
}
