#pragma once

#include "RenderCommand.h"

namespace Donut {

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

	public:
		inline static RendererAPI::API CurrentAPI() { return RendererAPI::GetAPI(); };
	};

}
