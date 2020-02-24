#include "dnpch.h"

#include "OpenGLContext.h"

#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Donut {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DN_CORE_ASSERT(status, "Failed to intitialize Glad!");

		DN_CORE_INFO("OpenGL Info:");
		DN_CORE_INFO(" - Vendor    {0}", glGetString(GL_VENDOR));
		DN_CORE_INFO(" - Renderer  {0}", glGetString(GL_RENDERER));
		DN_CORE_INFO(" - Version   {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}