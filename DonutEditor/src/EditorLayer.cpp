#include "EditorLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Donut {

	EditorLayer::EditorLayer()
		: Layer("DonutEditorLayer"), m_CameraController(1280.0f / 720.0f, true), m_World(b2Vec2(0.0f, -3.0f))
	{
	}

	void EditorLayer::OnAttach()
	{
		// TEXT RENDERING
		Font::Init();
		Font::LoadFont("assets/fonts/roboto.ttf", 480);

		m_Framebuffer = Framebuffer::Create(m_FramebufferProps);

		m_CheckerboardTexture = Texture2D::Create("assets/textures/checker_board.png");
		m_SpriteSheet = Texture2D::Create("assets/textures/tilemap.png");

		m_TextureStairs = SubTexture2D::CreateFromTexture(m_SpriteSheet, { 2.0f, 5.0f }, { 16.0f, 16.0f });
		m_TextureTree = SubTexture2D::CreateFromTexture(m_SpriteSheet, { 16.0f, 5.0f }, { 16.0f, 16.0f }, { 1.0f, 2.0f });

		// INITIALIZE PHYSICS OBJECTS
		// Ground
		b2PolygonShape groundBox;
		Box boxGround(&m_World, &groundBox, 0.0f, { 0.0f, -6.0f, 0.6f }, { 10.0f, 5.0f }, 0.0f, DN_COLOR_BLUE);
		m_Bodies.push_back(boxGround);

		// Dynamic box
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		Box boxDynamic(&m_World, &fixtureDef, { 0.0f, 4.0f, 0.6f }, { 1.0f, 1.0f }, 0.0f, DN_COLOR_RED);
		m_Bodies.push_back(boxDynamic);

		TransformComponent transform;
		transform.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });
		ColorComponent color;
		color.color = DN_COLOR_WHITE;


		m_Scene.MakeEntity(transform, color);
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		DN_PROFILE_FUNCTION();

		Renderer2D::ResetStats();
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);
		

		auto [x, y] = Input::GetRelativeMousePosition(m_CameraController);
		m_MouseX = x;
		m_MouseY = y;
		
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(DN_COLOR_PURPLE);
		RenderCommand::Clear();
		
		Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_Scene.Update(ts);
		Renderer2D::EndScene();
		/*
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		m_World.Step(ts, velocityIterations, positionIterations);
		
		
		Renderer2D::BeginScene(m_CameraController.GetCamera());
		Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, DN_COLOR_WHITE, 10);

		for (auto& body : m_Bodies)
		{
			body.Draw();
		}

		// TEXTURES
		Renderer2D::DrawQuad({ -1.0f,  3.0f, 0.3f }, { 1.0f, 1.0f }, m_TextureStairs);
		Renderer2D::DrawQuad({ 0.0f,  3.5f, 0.3f }, { 1.0f, 2.0f }, m_TextureTree);

		// SIMPLE LINE
		Renderer2D::DrawLine({ 0.0f,  0.0f }, { m_MouseX, m_MouseY }, 0.4f, DN_COLOR_BLACK, 0.03f);

		// CORDINATES GUIDE
		Renderer2D::DrawQuad({ 0.0f,  0.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		Renderer2D::DrawQuad({ 1.0f,  1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		Renderer2D::DrawQuad({ 0.0f,  1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);
		Renderer2D::DrawQuad({ 0.0f, -1.0f, 0.8f }, {  0.1f,  0.1f }, DN_COLOR_BLACK);

		Renderer2D::EndScene();

		// RENDER TEXT
		Renderer2D::BeginScene(m_CameraController.GetCamera(), true);
		Renderer2D::DrawTextLine("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", { -8.5f, -2.0f, 0.7f }, DN_COLOR_BLACK);
		Renderer2D::EndScene();
		*/
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
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

				if (ImGui::MenuItem("Close")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


			ImGui::Begin("Renderer2D Stats");
			auto stats = Renderer2D::GetStats();
			ImGui::Text("DrawCalls: %d", stats.DrawCalls);
			ImGui::Text("QuadCount: %d", stats.QuadCount);
			ImGui::Text("VertexCount: %d", stats.GetTotalVertexCount());
			ImGui::Text("IndexCount: %d", stats.GetTotalIndexCount());
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			ImGui::Begin("Viewport");
			ImGui::PopStyleVar();
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::GetImGuiLayer()->BlockEvents(!m_ViewportHovered);
			ImVec2 framebufferSize = ImGui::GetContentRegionAvail();
			if (framebufferSize.x != m_FramebufferProps.Width || framebufferSize.y != m_FramebufferProps.Height) {
				m_FramebufferProps.Width  = (uint32_t)framebufferSize.x;
				m_FramebufferProps.Height = (uint32_t)framebufferSize.y;
				m_Framebuffer->Resize(m_FramebufferProps.Width, m_FramebufferProps.Height);
				m_CameraController.Resize(m_FramebufferProps.Width, m_FramebufferProps.Height);
			}
			ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), { framebufferSize.x, framebufferSize.y }, {0, 1}, {1, 0});
			ImGui::End();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}