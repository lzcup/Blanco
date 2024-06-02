#include "BLpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Blanco
{
	VertexBuffer* VertexBuffer::CreatVertextBuffer(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RenderAPI::NONE:
			  BL_CORE_ASSERT(false, "API is none which is not supported!")
			  return nullptr;
		case RenderAPI::OPENGL:
		   	  return new OpenGLVertexBuffer(vertices, size);
		   default:
		   	  BL_CORE_ASSERT(false, "Unknown API is not supported!")
			  return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::CreatIndexBuffer(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI()) {
		case RenderAPI::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
		    return nullptr;
		case RenderAPI::OPENGL:
			return new OpenGLIndexBuffer(indices, count);
		default:
			BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
		}
	}
}
