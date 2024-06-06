#include "BLpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Blanco
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}