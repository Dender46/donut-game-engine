#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Donut::Texture2D::Create("assets/textures/checker_board.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Donut::Timestep ts)
{
	DN_PROFILE_FUNCTION();

	{
		DN_PROFILE_SCOPE("Camera::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		DN_PROFILE_SCOPE("RenderCommands");
		Donut::RenderCommand::SetClearColor(DN_COLOR_PURPLE);
		Donut::RenderCommand::Clear();
	}
	
	{
		DN_PROFILE_SCOPE("Renderer2D::Update");
		Donut::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Donut::Renderer2D::DrawQuad({ -0.3f, -0.3f, 0.0f }, { 0.8f, 0.8f }, m_BlueColor);
		Donut::Renderer2D::DrawRotatedQuad({ 0.5f, 0.3f, 0.0f }, { 0.4f, 0.8f }, glm::radians(30.0f), DN_COLOR_RED);
		Donut::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.99}, { 10.0f, 10.0f }, m_CheckerboardTexture, DN_COLOR_WHITE, 10);
		Donut::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	DN_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_BlueColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Donut::Event& e)
{
	m_CameraController.OnEvent(e);
}
