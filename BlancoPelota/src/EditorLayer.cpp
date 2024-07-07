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
#if 0
		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteComponent>(glm::vec4(1.0f,0.0f,0.0f,1.0f));
		auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
		greenSquare.AddComponent<SpriteComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
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
				transform.Translation.x = rand() % 5 + 2.0f;
			}
			virtual void OnUpdate(TimeStep ts) override {
				float moveSpeed = 10.0f;
				auto& transform = GetComponent<TransformComponent>();
				if (Input::IsKeyPressed(BL_KEY_W))
					transform.Translation.y += moveSpeed * ts;
				if (Input::IsKeyPressed(BL_KEY_S))
					transform.Translation.y -= moveSpeed * ts;
				if (Input::IsKeyPressed(BL_KEY_A))
					transform.Translation.x -= moveSpeed * ts;
				if (Input::IsKeyPressed(BL_KEY_D))
					transform.Translation.x += moveSpeed * ts;
			}
		};
		m_PrimaryCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
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
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BL_BIND_EVENT_FNC(EditorLayer::OnKeyPressed));
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
		ImGuiStyle& style= ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				ImGui::Separator();
				if (ImGui::MenuItem("New","Ctrl+N")) {
					NewFile();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Open As...", "Ctrl+O")) {
					OpenFileAs();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					SaveFileAs();
				}
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
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//short cuts
		if (e.GetRepeatCounts() > 0)
			return false;

		bool controlPressed = Input::IsKeyPressed(BL_KEY_LEFT_CONTROL) || Input::IsKeyPressed(BL_KEY_RIGHT_CONTROL);
		bool shiftPressed = Input::IsKeyPressed(BL_KEY_LEFT_SHIFT) || Input::IsKeyPressed(BL_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
		case BL_KEY_N:
			if (controlPressed) 
				NewFile();
			break;
		case BL_KEY_O:
			if (controlPressed)
				OpenFileAs();
			break;
		case BL_KEY_S:
			if (controlPressed && shiftPressed) 
				SaveFileAs();
			break;
		default:
			break;
		}
		return false;
	}
	void EditorLayer::NewFile()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnSetViewport((uint32_t)m_Viewport.x, (uint32_t)m_Viewport.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OpenFileAs()
	{
		std::string filepath = FileDialogs::OpenFile("Blanco Scene(*.blanco)\0*.blanco\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnSetViewport((uint32_t)m_Viewport.x, (uint32_t)m_Viewport.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			SceneSerializer sceneSerializer(m_ActiveScene);
			sceneSerializer.DeSerialize(filepath);
		}
	}
	void EditorLayer::SaveFileAs()
	{
		std::string filepath = FileDialogs::SaveFile("Blanco Scene(*.blanco)\0*.blanco\0");
		if (!filepath.empty())
		{
			SceneSerializer sceneSerializer(m_ActiveScene);
			sceneSerializer.Serialize(filepath);
		}
	}
}

