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

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

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
