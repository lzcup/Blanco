#include "BLpch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Blanco
{
	Shader* Shader::Create(std::string vertexSrc, std::string fragmentSrc)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}
}
