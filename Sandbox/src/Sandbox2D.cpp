#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Donut/Platform/OpenGL/OpenGLShader.h"


Sandbox2D::Sandbox2D()
	: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Donut::VertexArray::Create();
	float squareVertices[3 * 4] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f
	};
	Donut::Ref<Donut::VertexBuffer> squareVertexBuffer;
	squareVertexBuffer = Donut::VertexBuffer::Create(sizeof(squareVertices), squareVertices);
	squareVertexBuffer->SetLayout({
		{Donut::ShaderDataType::Float3, "a_Position"}
	});
	m_SquareVA->AddVertexBuffer(squareVertexBuffer);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Donut::Ref<Donut::IndexBuffer> squareIndexBuffer;
	squareIndexBuffer = Donut::IndexBuffer::Create(sizeof(squareIndices) / sizeof(uint32_t), squareIndices);
	m_SquareVA->SetIndexBuffer(squareIndexBuffer);

	m_FlatColorShader = Donut::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Donut::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Donut::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	Donut::RenderCommand::Clear();

	Donut::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Donut::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Donut::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Donut::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Donut::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Donut::Event& e)
{
	m_CameraController.OnEvent(e);
}
