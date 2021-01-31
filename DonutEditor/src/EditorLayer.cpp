#include "EditorLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Donut {

	EditorLayer::EditorLayer()
		: Layer("DonutEditorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		m_Framebuffer = Framebuffer::Create(m_FramebufferProps);

		// Add rendering system to ECS, otherwise it doesn't throw error and doesn't draw anything
		// TODO: remove static declaration
		static Renderer2DSystem rendererSystem;
		m_Scene.AddSystem(rendererSystem);

		// Square entity initialization
		ColorComponent color = { DN_COLOR_GREEN };

		// Create entity and add all components
		static Entity squareEntity = m_Scene.CreateEntity();
		squareEntity.AddComponent(color);

		m_MainCamera = m_Scene.CreateEntity("Main Camera");
		m_MainCamera.AddComponent(CameraComponent(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)));

		m_ClipSizeCamera = m_Scene.CreateEntity("Clip-Size Camera");
		m_ClipSizeCamera.AddComponent(CameraComponent(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f), false));
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		DN_PROFILE_FUNCTION();

		Renderer2D::ResetStats();
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);
		
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(DN_COLOR_PURPLE);
		RenderCommand::Clear();
		
		// Draw and update scene
		m_Scene.OnUpdate(ts);
		
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
			
			ImGui::Separator();
			if (ImGui::Checkbox("Use Main Camera", &m_UsingMainCamera))
			{
				m_MainCamera.GetComponent<CameraComponent>().IsPrimary = m_UsingMainCamera;
				m_ClipSizeCamera.GetComponent<CameraComponent>().IsPrimary = !m_UsingMainCamera;
			}

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