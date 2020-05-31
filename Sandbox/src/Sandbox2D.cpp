#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f, true), m_ParticleSystem(1000), m_Gravity(0.0f, -10.0f), m_World(m_Gravity)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Donut::Texture2D::Create("assets/textures/checker_board.png");
	m_SpriteSheet = Donut::Texture2D::Create("assets/textures/tilemap.png");
	
	m_TextureStairs	= Donut::SubTexture2D::CreateFromTexture(m_SpriteSheet, { 2.0f, 5.0f }, { 16.0f, 16.0f });
	m_TextureTree	= Donut::SubTexture2D::CreateFromTexture(m_SpriteSheet, { 16.0f, 5.0f }, { 16.0f, 16.0f }, { 1.0f, 2.0f });

	m_ParticleProps.Lifetime	= 1.0f;
	m_ParticleProps.Position	= { 0.0f, 0.0f };
	m_ParticleProps.ColorBegin	= { 255.0f / 255.0f, 194.0f / 255.0f, 144.0f / 255.0f, 1.0f };
	m_ParticleProps.ColorEnd	= { 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f };

	m_ParticleProps.SizeBegin		= 0.5f;
	m_ParticleProps.SizeEnd			= 0.0f;
	m_ParticleProps.SizeVariation	= 0.3f;
	
	m_ParticleProps.Velocity			= { 0.0f, 0.0f };
	m_ParticleProps.VelocityVariation	= { 3.0f, 1.0f };
	
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	m_GroundBody = m_World.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	m_GroundBody->CreateFixture(&groundBox, 0.0f);


	b2BodyDef dynamicBodyDef;
	dynamicBodyDef.type = b2_dynamicBody;
	dynamicBodyDef.position.Set(0.0f, 4.0f);
	m_DynamicBody = m_World.CreateBody(&dynamicBodyDef);

	b2PolygonShape box;
	box.SetAsBox(1.0f, 1.0f);
	m_DynamicBody->CreateFixture(&box, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
}

static float mouseX = 0;
static float mouseY = 0;

void Sandbox2D::OnUpdate(Donut::Timestep ts)
{
	DN_PROFILE_FUNCTION();

	Donut::Renderer2D::ResetStats();
	{
		DN_PROFILE_SCOPE("Camera::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		DN_PROFILE_SCOPE("Update Mouse Pos");
		auto [x, y] = Donut::Input::GetRelativeMousePosition(m_CameraController);
		mouseX = x;
		mouseY = y;
	}

	{
		DN_PROFILE_SCOPE("RenderCommands");
		Donut::RenderCommand::SetClearColor(DN_COLOR_PURPLE);
		Donut::RenderCommand::Clear();
	}

	{
		DN_PROFILE_SCOPE("Physics");
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		m_World.Step(ts, velocityIterations, positionIterations);
	}

	{
		DN_PROFILE_SCOPE("Renderer2D::Update");
		Donut::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Donut::Renderer2D::DrawQuad({  0.0f,  0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, DN_COLOR_WHITE, 10);

		b2Vec2 position = m_DynamicBody->GetPosition();
		float angle = m_DynamicBody->GetAngle();

		auto groundPos = m_GroundBody->GetPosition();
		auto groundRot = m_GroundBody->GetAngle();
		Donut::Renderer2D::DrawRotatedQuad({ groundPos.x, groundPos.y / 2, 0.5f }, {50.0f, 10.0f}, groundRot, DN_COLOR_RED);
		Donut::Renderer2D::DrawRotatedQuad({ position.x, position.y / 2, 0.5f }, {1.0f, 1.0f}, angle, DN_COLOR_BLUE);

		//Donut::Renderer2D::DrawQuad({  0.0f,  0.0f,  0.1f }, { 0.8f, 0.8f }, m_BlueColor);
		//Donut::Renderer2D::DrawRotatedQuad({ 0.5f, 0.3f, 0.2f}, { 0.4f, 0.8f }, glm::radians(30.0f), DN_COLOR_RED);
		
		//Donut::Renderer2D::DrawQuad({ -1.0f,  3.0f, 0.3f }, { 1.0f, 1.0f }, m_TextureStairs);
		//Donut::Renderer2D::DrawQuad({  0.0f,  3.5f, 0.3f }, { 1.0f, 2.0f }, m_TextureTree);

		
		//Donut::Renderer2D::DrawLine({ 0.0f,  0.0f }, { mouseX, mouseY }, 0.4f, DN_COLOR_BLACK, 0.03f);

		// just coordination guides
		//Donut::Renderer2D::DrawQuad({ 0.0f,  0.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		//Donut::Renderer2D::DrawQuad({ 1.0f,  1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		//Donut::Renderer2D::DrawQuad({ 0.0f,  1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		//Donut::Renderer2D::DrawQuad({ 0.0f, -1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);

		Donut::Renderer2D::EndScene();
	}

	// TODO: Check memory leaks
	
	if (Donut::Input::IsMouseButtonPressed(DN_MOUSE_BUTTON_LEFT))
	{
		m_ParticleProps.Position = { mouseX, mouseY };
		
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
