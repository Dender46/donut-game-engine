#include "dnpch.h"

#include "RenderCommand.h"
#include "Donut/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Donut {

	Ref<RendererAPI> RenderCommand::s_RendererAPI = std::make_shared<OpenGLRendererAPI>();

}