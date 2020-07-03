#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f, true), m_ParticleSystem(1000), m_World(b2Vec2(0.0f, -3.0f))
{
}

static Donut::Ref<Donut::Texture2D> fontTexture;

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
	
	// INITIALIZE PHYSICS OBJECTS
	// Ground
	b2PolygonShape groundBox;
	Donut::Box boxGround(&m_World, &groundBox, 0.0f, { 0.0f, -6.0f, 0.6f }, { 10.0f, 5.0f }, 0.0f, DN_COLOR_BLUE);
	m_Bodies.push_back(boxGround);
	

	// Dynamic box
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	Donut::Box boxDynamic(&m_World, &fixtureDef, { 0.0f, 4.0f, 0.6f }, { 1.0f, 1.0f }, 0.0f, DN_COLOR_RED);
	m_Bodies.push_back(boxDynamic);

	// TEXT RENDERING
	Donut::Font::Init();
	Donut::Font::LoadFont("assets/fonts/roboto.ttf", 480);

	fontTexture = Donut::Font::GetChar('X').get()->Texture;
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
		DN_PROFILE_SCOPE("Update Mouse Pos");
		auto [x, y] = Donut::Input::GetRelativeMousePosition(m_CameraController);
		m_MouseX = x;
		m_MouseY = y;
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
		
		for (auto& body : m_Bodies)
		{
			body.Draw();
		}
		
		// TEXTURES
		Donut::Renderer2D::DrawQuad({ -1.0f,  3.0f, 0.3f }, { 1.0f, 1.0f }, m_TextureStairs);
		Donut::Renderer2D::DrawQuad({  0.0f,  3.5f, 0.3f }, { 1.0f, 2.0f }, m_TextureTree);

		// SIMPLE LINE
		//Donut::Renderer2D::DrawLine({ 0.0f,  0.0f }, { m_MouseX, m_MouseY }, 0.4f, DN_COLOR_BLACK, 0.03f);

		// CORDINATES GUIDE
		//Donut::Renderer2D::DrawQuad({ 0.0f,  0.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		//Donut::Renderer2D::DrawQuad({ 1.0f,  1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		//Donut::Renderer2D::DrawQuad({ 0.0f,  1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		//Donut::Renderer2D::DrawQuad({ 0.0f, -1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);

		Donut::Renderer2D::EndScene();


		// RENDER TEXT
		Donut::Renderer2D::BeginScene(m_CameraController.GetCamera(), true);
		//Donut::Renderer2D::DrawTextLine("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", { -10.0f, 1.0f, 0.0f }, DN_COLOR_BLACK);
		Donut::Renderer2D::DrawTextLine("PQ", { 1.0f, 1.0f, 0.0f }, DN_COLOR_BLACK);

		//Donut::Renderer2D::DrawQuad({ 0.0f,  0.0f, 0.8f }, { 1.0f, 1.0f }, fontTexture, DN_COLOR_WHITE, 1.0f);
		Donut::Renderer2D::EndScene();
	}

	// TODO: Check memory leaks
	
	if (Donut::Input::IsMouseButtonPressed(DN_MOUSE_BUTTON_LEFT))
	{
		m_ParticleProps.Position = { m_MouseX, m_MouseY };
		
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
