#pragma once

#include "Shader.h"
#include "Camera.h"
#include "RenderCommand.h"

namespace Donut {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

	public:
		inline static RendererAPI::API CurrentAPI() { return RendererAPI::GetAPI(); };
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}
