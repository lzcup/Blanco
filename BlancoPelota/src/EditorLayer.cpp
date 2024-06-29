#include "EditorLayer.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"

namespace Blanco
{
	EditorLayer::EditorLayer() :Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		FrameBufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(spec);

		m_Texture = Texture2D::Create("assets/textures/cat.png");
		m_DoorsSpriteSheet = Texture2D::Create("assets/game/textures/Doors.png");
		m_YellowDoor = SubTexture2D::CreateTextureByCoords(m_DoorsSpriteSheet, { 7,0 }, { 128.0f,128.0f }, { 1,2 });
		m_YellowKey = SubTexture2D::CreateTextureByCoords(m_DoorsSpriteSheet, { 8,2 }, { 128.0f,128.0f }, { 1,1 });
		m_OpenDoor = SubTexture2D::CreateTextureByCoords(m_DoorsSpriteSheet, { 5,0 }, { 128.0f,128.0f }, { 1,2 });
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		BL_PROFILE_FUNCTION();

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();
		{
			BL_PROFILE_SCOPE("CameraController Update");
			m_CameraController.OnUpdate(ts);
		}

		{
			BL_PROFILE_SCOPE("BufferClear");
			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCommand::Clear();
		}

		{
			BL_PROFILE_SCOPE("Render Scene");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawQuad({ 0.0f,0.0f }, { 0.3f,0.3f }, m_FlatColor);
			Renderer2D::DrawQuad({ 1.0f,0.0f }, { 1.0f,1.5f }, { 0.8f,0.5f,0.5f,1.0f });
			Renderer2D::DrawRotateQuad({ -0.8f,-0.8f }, glm::radians(105.0f), { 0.5f,0.8f }, { 0.2f,0.9f,0.5f,1.0f });
			Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 20.0f,20.0f }, m_Texture, { 0.8f,0.8f,0.8f,0.8f }, 20.0f);
			Renderer2D::DrawRotateQuad({ 0.0f,0.0f,-0.05f }, glm::radians(45.0f), { 5.0f,5.0f }, m_Texture, { 0.2f,0.8f,0.8f,0.8f }, 1.0f);
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f) {
				for (float x = -5.0f; x < 5.0f; x += 0.5f) {
					glm::vec4 color = { 0.1f * x + 0.5f,0.1f * y + 0.5f,0.6f,0.7f };
					Renderer2D::DrawQuad({ x,y }, { 0.45f,0.45f }, color);
				}
			}
			Renderer2D::EndScene();
			m_FrameBuffer->UnBind();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void EditorLayer::OnImguiRender()
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static bool dockspaceOpen = true;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();

		auto& stats = Renderer2D::GetStats();
		ImGui::Begin("Statistics");
		ImGui::Text("DrawCalls:%d", stats.DrawCalls);
		ImGui::Text("DrawQuads:%d", stats.DrawQuads);
		ImGui::Text("DrawVertices:%d", stats.DrawVertices);
		ImGui::Text("DrawIndices:%d", stats.DrawIndices);
		ImGui::ColorEdit4("Square Color", &m_FlatColor.r);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewpoint");
		ImVec2 regionViewport = ImGui::GetContentRegionAvail();
		if (m_Viewport != *((glm::vec2*)&regionViewport)) {
			m_FrameBuffer->Resize((uint32_t)regionViewport.x, (uint32_t)regionViewport.y);
		}
		m_Viewport = { regionViewport.x,regionViewport.y };
		m_CameraController.OnResize(regionViewport.x, regionViewport.y);
		uint32_t textureID = m_FrameBuffer->GetColorAttchmentRendererID();
		ImGui::Image((void*)textureID, { regionViewport.x,regionViewport.y },{0,1},{1,0});
		ImGui::PopStyleVar();
		ImGui::End();
	}
}

