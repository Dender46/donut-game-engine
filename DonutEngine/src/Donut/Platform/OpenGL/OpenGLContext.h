#pragma once

#include "Donut\Renderer\RenderingContext.h"

// forward declaration
struct GLFWwindow;

namespace Donut {

	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
	
}
