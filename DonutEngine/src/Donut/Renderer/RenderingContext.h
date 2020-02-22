#pragma once

#include "dnpch.h"

namespace Donut {

	class RenderingContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}