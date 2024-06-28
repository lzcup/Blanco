#include "BLpch.h"
#include "FrameBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Blanco
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLFrameBuffer>(spec);
		}
		BL_CORE_ASSERT(false, "Unknown API is not supported!")
			return nullptr;
	}
}

