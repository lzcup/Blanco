#include "EditorLayer.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"
#include <gtc/type_ptr.hpp>

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

		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteComponent>(glm::vec4(1.0f,0.0f,0.0f,1.0f));
		m_PrimaryCamera = m_ActiveScene->CreateEntity("Primary Camera");
		m_PrimaryCamera.AddComponent<CameraComponent>();
		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
		m_SecondCamera.AddComponent<CameraComponent>();
		m_SecondCamera.GetComponent<CameraComponent>().Primary = false;

		m_Texture = Texture2D::Create("assets/textures/cat.png");
		m_DoorsSpriteSheet = Texture2D::Create("assets/game/textures/Doors.png");
		m_YellowDoor = SubTexture2D::CreateTextureByCoords(m_DoorsSpriteSheet, { 7,0 }, { 128.0f,128.0f }, { 1,2 });
		m_YellowKey = SubTexture2D::CreateTextureByCoords(m_DoorsSpriteSheet, { 8,2 }, { 128.0f,128.0f }, { 1,1 });
		m_OpenDoor = SubTexture2D::CreateTextureByCoords(m_DoorsSpriteSheet, { 5,0 }, { 128.0f,128.0f }, { 1,2 });

		class CameraController :public ScriptableEntity {
		public:
			virtual void OnCreate() override{
				auto& transform = GetComponent<TransformComponent>();
				transform.Transform[3][0] = rand() % 5 + 2.0f;
			}
			virtual void OnUpdate(TimeStep ts) override {
				float moveSpeed = 10.0f;
				auto& transform = GetComponent<TransformComponent>();
				if (Input::IsKeyPressed(BL_KEY_W))
					transform.Transform[3][1] += moveSpeed * ts;
				if (Input::IsKeyPressed(BL_KEY_S))
					transform.Transform[3][1] -= moveSpeed * ts;
				if (Input::IsKeyPressed(BL_KEY_A))
					transform.Transform[3][0] -= moveSpeed * ts;
				if (Input::IsKeyPressed(BL_KEY_D))
					transform.Transform[3][0] += moveSpeed * ts;
			}
		};
		m_PrimaryCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		Renderer2D::ResetStats();

		if(FrameBufferSpecification& spec=m_FrameBuffer->GetSpecification();
			spec.Width > 0 && spec.Height > 0 && (spec.Width != m_Viewport.x || spec.Height != m_Viewport.y) ){
			m_FrameBuffer->Resize((uint32_t)m_Viewport.x, (uint32_t)m_Viewport.y);
			m_ActiveScene->OnSetViewport((uint32_t)m_Viewport.x, (uint32_t)m_Viewport.y);
		}

		m_FrameBuffer->Bind();
		if (m_ViewFocuse)
			m_CameraController.OnUpdate(ts);
		
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();;

		m_ActiveScene->OnUpdate(ts);

		m_FrameBuffer->UnBind();
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

		m_SceneHierarchyPanel.OnImguiRender();

		auto& stats = Renderer2D::GetStats();
		ImGui::Begin("Statistics");
		if (m_SquareEntity) {
			auto& color = m_SquareEntity.GetComponent<SpriteComponent>().Color;
			ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());
			ImGui::ColorEdit4("Square Color", &color.r);
		}
		if (m_PrimaryCamera && m_SecondCamera) {
			glm::mat4& transform = m_PrimaryCamera.GetComponent<TransformComponent>();
			bool* primary = &(m_PrimaryCamera.GetComponent<CameraComponent>().Primary);
			ImGui::DragFloat3("Primary Transform", glm::value_ptr(transform[3]));
			if (ImGui::Checkbox("Primary Camera", primary)) {
				m_SecondCamera.GetComponent<CameraComponent>().Primary = !*primary;
			}
			float size = m_SecondCamera.GetComponent<CameraComponent>().Camera.GetOrthoGraphicSize();
			ImGui::DragFloat("Second OrthoSize", &size);
			m_SecondCamera.GetComponent<CameraComponent>().Camera.SetOrthGraphicSize(size);
		}

		ImGui::Text("DrawCalls:%d", stats.DrawCalls);
		ImGui::Text("DrawQuads:%d", stats.DrawQuads);
		ImGui::Text("DrawVertices:%d", stats.DrawVertices);
		ImGui::Text("DrawIndices:%d", stats.DrawIndices);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewpoint");
		m_ViewFocuse = ImGui::IsWindowFocused();
		m_ViewHovered = ImGui::IsWindowHovered();
		Application::Get().GetImgui().BlockEvents(!(m_ViewFocuse && m_ViewHovered));
		ImVec2 regionViewport = ImGui::GetContentRegionAvail();
		m_Viewport = { regionViewport.x,regionViewport.y };
		uint32_t textureID = m_FrameBuffer->GetColorAttchmentRendererID();
		ImGui::Image((void*)(uint64_t)textureID, { m_Viewport.x,m_Viewport.y },{0,1},{1,0});
		ImGui::PopStyleVar();
		ImGui::End();
	}
}

