#include "dnpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"

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
		const uint32_t MaxQuads		= 10000;
		const uint32_t MaxVertices	= MaxQuads * 4;
		const uint32_t MaxIndices	= MaxQuads * 6;

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount	= 0;
		QuadVertex* QuadVBBase	= nullptr;
		QuadVertex* QuadVBPtr	= nullptr;

		static const uint32_t MaxTextureSlots = 32;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 == white texture
	};

	static Renderer2DObject s_Data;

	void Renderer2D::Init()
	{
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

		uint32_t whiteColor = 0xffffffff;
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.WhiteTexture->SetData(&whiteColor, sizeof(uint32_t));
		
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;
		
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
		
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVBPtr = s_Data.QuadVBBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVBPtr - (uint8_t*)s_Data.QuadVBBase;
		s_Data.QuadVB->SetData(s_Data.QuadVBBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		const uint32_t textureIndex = 0;
		const float tilingAmount = 1.0f;

		s_Data.QuadVBPtr->Position = position;
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoords = { 0.0f, 0.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoords = { 1.0f, 0.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoords = { 1.0f, 1.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVBPtr->Color = color;
		s_Data.QuadVBPtr->TexCoords = { 0.0f, 1.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadIndexCount += 6;
		/*
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);
		 
		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
		*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tilingAmount);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		float textureIndex = 0;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadVBPtr->Position = position;
		s_Data.QuadVBPtr->Color = tint;
		s_Data.QuadVBPtr->TexCoords = { 0.0f, 0.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVBPtr->Color = tint;
		s_Data.QuadVBPtr->TexCoords = { 1.0f, 0.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVBPtr->Color = tint;
		s_Data.QuadVBPtr->TexCoords = { 1.0f, 1.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadVBPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVBPtr->Color = tint;
		s_Data.QuadVBPtr->TexCoords = { 0.0f, 1.0f };
		s_Data.QuadVBPtr->TexIndex = textureIndex;
		s_Data.QuadVBPtr->TilingAmount = tilingAmount;
		s_Data.QuadVBPtr++;

		s_Data.QuadIndexCount += 6;

		/*
		texture->Bind();
		s_Data.TextureShader->SetFloat("u_TilingAmount", tilingAmount);
		s_Data.TextureShader->SetFloat4("u_Color", tint);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
		*/
	}



	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tilingAmount);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4 tint, const float tilingAmount)
	{
		texture->Bind();
		s_Data.TextureShader->SetFloat("u_TilingAmount", tilingAmount);
		s_Data.TextureShader->SetFloat4("u_Color", tint);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
	}

}