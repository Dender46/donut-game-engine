#include "dnpch.h"

#include "RenderCommand.h"
#include "Donut/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Donut {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}