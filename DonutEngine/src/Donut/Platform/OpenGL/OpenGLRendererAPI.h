#pragma once

#include <glm/glm.hpp>
#include "Donut/Renderer/RendererAPI.h"
#include "Donut/Renderer/VertexArray.h"

namespace Donut {

	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};

}
