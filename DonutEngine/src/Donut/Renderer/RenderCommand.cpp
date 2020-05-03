#include "dnpch.h"

#include "RenderCommand.h"
#include "Donut/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Donut {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();

}