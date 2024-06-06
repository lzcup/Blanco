#include "BLpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Blanco 
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RenderAPI::NONE:
			BL_CORE_ASSERT(false, "API is none which is not supported!")
				return nullptr;
		case RenderAPI::OPENGL:
			return new OpenGLVertexArray();
		}
	    BL_CORE_ASSERT(false, "Unknown API is not supported!")
		    return nullptr;
	}
}




