#include "BLpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Blanco
{
	Ref<VertexBuffer> VertexBuffer::CreatVertextBuffer(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			  BL_CORE_ASSERT(false, "API is none which is not supported!")
			  return nullptr;
		case RendererAPI::API::OPENGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::CreatIndexBuffer(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
		    return nullptr;
		case RendererAPI::API::OPENGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}
}
