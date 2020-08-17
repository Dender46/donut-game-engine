#include "dnpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Donut/Systems/Font.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Donut {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
		float TilingAmount;
	};

	struct Renderer2DObject {
		static const uint32_t MaxQuads		= 10000;
		static const uint32_t MaxVertices	= MaxQuads * 4;
		static const uint32_t MaxIndices	= MaxQuads * 6;

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader, TextShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount	= 0;
		QuadVertex* QuadVBBase	= nullptr;
		QuadVertex* QuadVBPtr	= nullptr;

		static const uint32_t MaxTextureSlots = 32;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 == white texture

		glm::vec4 QuadVertexPositions[4];
		glm::vec2 QuadTextureCoords[4];
		
		Renderer2D::Statistics Stats;
	};

	static Renderer2DObject s_Data;

	void Renderer2D::Init()
	{
		DN_PROFILE_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();

		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Float,	 "a_TexIndex"},
			{ShaderDataType::Float,	 "a_TilingAmount"},
		});
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVBBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset;
			
			offset += 4;
		}
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;

		// Creating white texture for pure-color quads
		uint32_t whiteColor = 0xffffffff;
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.WhiteTexture->SetData(&whiteColor, sizeof(uint32_t));
		
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;
		
		// Creating texture
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextShader = Shader::Create("assets/shaders/TextShader.glsl");
		s_Data.TextShader->Bind();
		s_Data.TextShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Assign default white texture
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Predefined coords of vertices and textures
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.QuadTextureCoords[0] = { 0.0f, 0.0f };
		s_Data.QuadTextureCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadTextureCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadTextureCoords[3] = { 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		DN_PROFILE_FUNCTION();

		delete[] s_Data.QuadVBBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera, bool isText)
	{
		DN_PROFILE_FUNCTION();

		if (isText)
		{
			s_Data.TextShader->Bind();
			s_Data.TextShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		}
		else
		{
			s_Data.TextureShader->Bind();
			s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		}

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVBPtr = s_Data.QuadVBBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		DN_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t) ((uint8_t*)s_Data.QuadVBPtr - (uint8_t*)s_Data.QuadVBBase);
		s_Data.QuadVB->SetData(s_Data.QuadVBBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		DN_PROFILE_FUNCTION()

		if (s_Data.QuadIndexCount == 0)
			return; // Nothing to draw

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);

		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVBPtr = s_Data.QuadVBBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		DN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, tint, texture, tilingAmount);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		DN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, tint, texture->GetTexture(), tilingAmount, texture->GetTextureCoords());
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		DN_PROFILE_FUNCTION();
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVBPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVBPtr->Color = color;
			s_Data.QuadVBPtr->TexCoords = s_Data.QuadTextureCoords[i];
			s_Data.QuadVBPtr->TexIndex = 0;
			s_Data.QuadVBPtr->TilingAmount = 1.0f;
			s_Data.QuadVBPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// Increment statistics
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const float tilingAmount, const glm::vec2* texCoords)
	{
		DN_PROFILE_FUNCTION();
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		float textureIndex = 0;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// Save texture for future quads
		if (textureIndex == 0)
		{
			if (s_Data.TextureSlotIndex == s_Data.MaxTextureSlots)
				FlushAndReset();
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// Texture coords
		if (texCoords == nullptr)
			texCoords = s_Data.QuadTextureCoords;

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVBPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVBPtr->Color = color;
			s_Data.QuadVBPtr->TexCoords = texCoords[i];
			s_Data.QuadVBPtr->TexIndex = textureIndex;
			s_Data.QuadVBPtr->TilingAmount = tilingAmount;
			s_Data.QuadVBPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// Increment statistics
		s_Data.Stats.QuadCount++;
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		DN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale (glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		DN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale (glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, tint, texture, tilingAmount);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<SubTexture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		DN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, tint, texture->GetTexture(), tilingAmount, texture->GetTextureCoords());
	}

	void Renderer2D::DrawLine(const glm::vec2& p1, const glm::vec2& p2, const float z, const glm::vec4& color, const float thickness)
	{
		DN_PROFILE_FUNCTION();

		glm::vec3 position = { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2, z };
		float rotation = glm::atan(p2.y - p1.y, p2.x - p1.x);
		float length = glm::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { length, thickness, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawTextLine(const std::string& text, const glm::vec3& position, const glm::vec4& color)
	{
		DN_PROFILE_FUNCTION();

		float x = position.x;
		float y = position.y;
		float scale = 0.001f;

		for (char ch : text)
		{
			const Ref<Font::Character> chInfo = Font::GetChar(ch);
			 
			float xpos = x + chInfo->Bearing.x * scale;
			float ypos = y - (chInfo->Size.y - chInfo->Bearing.y) * scale;
			xpos += chInfo->Texture->GetWidth() / 2.0f * scale;
			ypos += chInfo->Texture->GetHeight() / 2.0f * scale;

			float w = chInfo->Size.x * scale;
			float h = chInfo->Size.y * scale;
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), {xpos, ypos, position.z}) * glm::scale(glm::mat4(1.0f), { w, -h, 1.0f });
			DrawQuad(transform, color, chInfo->Texture, 1.0f);

			x += (chInfo->Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}