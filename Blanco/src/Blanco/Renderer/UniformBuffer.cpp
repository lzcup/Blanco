#include "BLpch.h"
#include "UniformBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Blanco
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLUniformBuffer>(size, binding);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}
}

