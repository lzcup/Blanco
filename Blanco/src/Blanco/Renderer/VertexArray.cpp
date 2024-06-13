#include "BLpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Blanco 
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RendererAPI::API::OPENGL:
			return std ::make_shared<OpenGLVertexArray>();
		}
	    BL_CORE_ASSERT(false, "Unknown API is not supported!")
		    return nullptr;
	}
}




