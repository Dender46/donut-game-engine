#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox"), m_CameraController(1280.0f / 720.0f, true), m_ParticleSystem(1000), m_World(b2Vec2(0.0f, -3.0f))
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

	m_Framebuffer = Donut::Framebuffer::Create(m_FramebufferProps);
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
		m_Framebuffer->Bind();
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
		Donut::Renderer2D::DrawTextLine("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", { -8.5f, -2.0f, 0.7f }, DN_COLOR_BLACK);

		//Donut::Renderer2D::DrawQuad({ 0.0f,  0.0f, 0.8f }, { 1.0f, 1.0f }, fontTexture, DN_COLOR_WHITE, 1.0f);
		Donut::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	// TODO: Check memory leaks
	
	if (Donut::Input::IsMouseButtonPressed(DN_MOUSE_BUTTON_LEFT))
	{
		m_ParticleProps.Position = { m_MouseX, m_MouseY };
		
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_ParticleProps);
	}


	//m_ParticleSystem.OnUpdate(ts);
	//m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	
}

void Sandbox2D::OnImGuiRender()
{
	DN_PROFILE_FUNCTION();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Close")) Donut::Application::Get().Close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


		ImGui::Begin("Settings");

		auto stats = Donut::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("DrawCalls: %d", stats.DrawCalls);
		ImGui::Text("QuadCount: %d", stats.QuadCount);
		ImGui::Text("VertexCount: %d", stats.GetTotalVertexCount());
		ImGui::Text("IndexCount: %d", stats.GetTotalIndexCount());
		ImGui::Text("--------------------");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_BlueColor));
		ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), { m_FramebufferProps.Width, m_FramebufferProps.Height }, {5, 5});

		ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Donut::Event& e)
{
	m_CameraController.OnEvent(e);
}
