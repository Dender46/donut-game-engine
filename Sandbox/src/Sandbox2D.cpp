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
	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Donut::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Donut::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	Donut::RenderCommand::Clear();
	
	Donut::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Donut::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
	Donut::Renderer2D::EndScene();
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
