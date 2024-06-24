#include "BLpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Blanco
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLTexture2D>(path);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
    }
}
