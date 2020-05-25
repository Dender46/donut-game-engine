#pragma once

#include "Donut/Core/Core.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Donut {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<SubTexture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<SubTexture2D>& texture, const glm::vec4 tint = glm::vec4(1.0f), const float tilingAmount = 1.0f);
	
		static void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const float z, const glm::vec4& color, const float thickness = 0.05f);

		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset(); // to handle multiple draw calls if necessary 
		static void AddDataToVertexBuffer(const glm::mat4& transform, const glm::vec4& color);
		static void AddDataToVertexBuffer(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const float tilingAmount, const glm::vec2* texCoords = nullptr);
	};

}