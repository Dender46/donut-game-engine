#pragma once

#include "Donut/Core/Core.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

namespace Donut {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f }, const float textureScale = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f }, const float textureScale = 1.0f);
	};

}