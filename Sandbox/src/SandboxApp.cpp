#include <Donut.h>
#include <Donut/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Donut/Platform/OpenGL/OpenGLShader.h"


#include "Sandbox2D.h"

/*
class OpenGLExampleLayer : public Donut::Layer
{
public:
	OpenGLExampleLayer()
		: Layer("OpenGLExampleLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
		m_TriangleVA = Donut::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f,  0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Donut::Ref<Donut::VertexBuffer> vertexBuffer;
		vertexBuffer = Donut::VertexBuffer::Create(sizeof(vertices), vertices);
		vertexBuffer->SetLayout({
			{ Donut::ShaderDataType::Float3, "a_Position" },
			{ Donut::ShaderDataType::Float4, "a_Color" },
		});
		m_TriangleVA->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Donut::Ref<Donut::IndexBuffer> indexBuffer;
		indexBuffer = Donut::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);
		m_TriangleVA->SetIndexBuffer(indexBuffer);



		m_SquareVA = Donut::VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f
		};
		Donut::Ref<Donut::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = Donut::VertexBuffer::Create(sizeof(squareVertices), squareVertices);
		squareVertexBuffer->SetLayout({
			{Donut::ShaderDataType::Float3, "a_Position"},
			{Donut::ShaderDataType::Float2, "a_TexCoords"}
		});
		m_SquareVA->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Donut::Ref<Donut::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = Donut::IndexBuffer::Create(sizeof(squareIndices) / sizeof(uint32_t), squareIndices);
		m_SquareVA->SetIndexBuffer(squareIndexBuffer);

		// Creating shader with shader library
		m_ShaderLib.Load("assets/shaders/VertexPosition.glsl");
		m_ShaderLib.Load("assets/shaders/FlatColor.glsl");
		auto textureShader = m_ShaderLib.Load("assets/shaders/Texture.glsl");

		// adding texture to shader
		m_Texture = Donut::Texture2D::Create("assets/textures/checker_board.png");
		std::dynamic_pointer_cast<Donut::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Donut::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0); // 0 - texture slot

		m_ExplosionIconTexture = Donut::Texture2D::Create("assets/textures/explosion_icon.png");
	}

	void OnUpdate(Donut::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Donut::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Donut::RenderCommand::Clear();

		Donut::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto flatColorShader = m_ShaderLib.Get("FlatColor");
		std::dynamic_pointer_cast<Donut::OpenGLShader>(flatColorShader)->Bind();
		std::dynamic_pointer_cast<Donut::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int x = 0; x < 20 ; x++)
			for (int y = 0; y < 20; y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Donut::Renderer::Submit(flatColorShader, m_SquareVA, transform);
			}

		auto textureShader = m_ShaderLib.Get("Texture");

		m_Texture->Bind();
		Donut::Renderer::Submit(textureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
		m_ExplosionIconTexture->Bind();
		Donut::Renderer::Submit(textureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
		//Donut::Renderer::Submit(m_RainbowShader, m_TriangleVA);

		Donut::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Donut::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	private:
		Donut::OrthographicCameraController m_CameraController;
		
		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.9f };

		Donut::ShaderLibrary m_ShaderLib;
		Donut::Ref<Donut::VertexArray> m_TriangleVA, m_SquareVA;

		Donut::Ref<Donut::Texture2D> m_Texture, m_ExplosionIconTexture;
};
*/
class Sandbox : public Donut::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
};

Donut::Application* Donut::CreateApplication()
{
	return new Sandbox();
}