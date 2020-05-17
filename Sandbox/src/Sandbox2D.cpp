#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f, true), m_ParticleSystem(1000)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Donut::Texture2D::Create("assets/textures/checker_board.png");

	m_ParticleProps.Lifetime	= 1.0f;
	m_ParticleProps.Position	= { 0.0f, 0.0f };
	m_ParticleProps.ColorBegin	= { 255.0f / 255.0f, 194.0f / 255.0f, 144.0f / 255.0f, 1.0f };
	m_ParticleProps.ColorEnd	= { 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f };

	m_ParticleProps.SizeBegin		= 0.5f;
	m_ParticleProps.SizeEnd			= 0.0f;
	m_ParticleProps.SizeVariation	= 0.3f;
	
	m_ParticleProps.Velocity			= { 0.0f, 0.0f };
	m_ParticleProps.VelocityVariation	= { 3.0f, 1.0f };
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Donut::Timestep ts)
{
	DN_PROFILE_FUNCTION();

	Donut::Renderer2D::ResetStats();
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


	if (Donut::Input::IsMouseButtonPressed(DN_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Donut::Input::GetMousePosition();
		auto width = Donut::Application::Get().GetWindow().GetWidth();
		auto height = Donut::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_ParticleProps.Position = { x + pos.x, y + pos.y };
		
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_ParticleProps);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
	DN_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Donut::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("QuadCount: %d", stats.QuadCount);
	ImGui::Text("VertexCount: %d", stats.GetTotalVertexCount());
	ImGui::Text("IndexCount: %d", stats.GetTotalIndexCount());
	ImGui::Text("--------------------");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_BlueColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Donut::Event& e)
{
	m_CameraController.OnEvent(e);
}
