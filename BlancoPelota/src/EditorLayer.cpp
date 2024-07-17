#include "EditorLayer.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"
#include <gtc/type_ptr.hpp>
#include "ImGuizmo.h"
#include "Blanco/Math/Math.h"

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
		spec.Attachments = { FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::RED_INTEGER,FrameBufferTextureFormat::DEPTH24STENCIL8 };
		m_FrameBuffer = FrameBuffer::Create(spec);

		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.DeSerialize(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_PlayIcon = Texture2D::Create("Resource/Icons/play_circle_icon.png");
		m_StopIcon = Texture2D::Create("Resource/Icons/stop.png");

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
			spec.Width > 0 && spec.Height > 0 && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y) ){
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnSetViewport((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_FrameBuffer->Bind();

	
		
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();;

		//clear attachment to -1
		m_FrameBuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
		case SceneState::Edit:
			if (m_ViewFocuse)
				m_CameraController.OnUpdate(ts);
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnEditorUpdate(ts, m_EditorCamera);
			break;
		case SceneState::Runtime:
			m_ActiveScene->OnRuntimeUpdate(ts);
			break;
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportMinBound.x;
		my -= m_ViewportMinBound.y;
		my = m_ViewportSize.y - my;  //reverse
		if (mx >= 0 && my >= 0 && mx < m_ViewportSize.x && my < m_ViewportSize.y) {
			int pixelData= m_FrameBuffer->ReadPixel(1, (int)mx, (int)my);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BL_BIND_EVENT_FNC(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BL_BIND_EVENT_FNC(EditorLayer::OnMouseButtonPressed));
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
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
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
					NewScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Open As...", "Ctrl+O")) {
					OpenScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					SaveSceneAs();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					SaveScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImguiRender();
		m_ContentBrowserPanel.OnImguiRender();

		auto& stats = Renderer2D::GetStats();
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Begin("Statistics");
		ImGui::Text("Entity:%s", name.c_str());
		ImGui::Text("DrawCalls:%d", stats.DrawCalls);
		ImGui::Text("DrawQuads:%d", stats.DrawQuads);
		ImGui::Text("DrawVertices:%d", stats.DrawVertices);
		ImGui::Text("DrawIndices:%d", stats.DrawIndices);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewpoint");

		m_ViewFocuse = ImGui::IsWindowFocused();
		m_ViewHovered = ImGui::IsWindowHovered();
		Application::Get().GetImgui().BlockEvents(!m_ViewFocuse && !m_ViewHovered);

		auto viewportOffset = ImGui::GetCursorPos();//include tab bar
		auto minBound = ImGui::GetWindowPos(); //window upper left corner
		m_ViewportMinBound.x = minBound.x + viewportOffset.x;
		m_ViewportMinBound.y = minBound.y + viewportOffset.y;

		ImVec2 regionViewport = ImGui::GetContentRegionAvail();
		m_ViewportSize = { regionViewport.x,regionViewport.y };
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();

		ImGui::Image((void*)(uint64_t)textureID, { m_ViewportSize.x,m_ViewportSize.y },{0,1},{1,0});

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}

			ImGui::EndDragDropTarget();
		}

		//Gizmos
		Entity seletedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (seletedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			//RunTimeCamera
		/*	auto cameraEntity = m_ActiveScene->GetPrimaryCamera();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4 projection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

			//EditorCamera
			const glm::mat4 projection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			//Entity transform
			auto& tc = seletedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			//snapping
			float snapValue = 0.5f;
			if (m_GuizmoOperation == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;
			float snapValues[3] = {snapValue,snapValue,snapValue};
			bool isSnap = Input::IsKeyPressed(BL_KEY_LEFT_CONTROL);

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(projection),
				(ImGuizmo::OPERATION)m_GuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, isSnap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, roation, scale;
				Math::DecomposeTransform(transform, translation, roation, scale);

				tc.Translation = translation;
				glm::vec3 deltaRotation = roation - tc.Rotation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

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
				NewScene();
			break;
		case BL_KEY_O:
			if (controlPressed)
				OpenScene();
			break;
		case BL_KEY_S:
			if (controlPressed && shiftPressed) 
				SaveSceneAs();
			if (controlPressed && !shiftPressed)
				SaveScene();
			break;
	    //Gizmos
		case BL_KEY_Q:
			m_GuizmoOperation = -1;
			break;
		case BL_KEY_W:
			m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case BL_KEY_E:
			m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;
			break;
		case BL_KEY_R:
			m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;
			break;
		//Duplicate
		case BL_KEY_D:
			if (controlPressed)
				OnDuplicateEntity();
			break;
		default:
			break;
		}
		return false;
	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseCode() == BL_MOUSE_BUTTON_LEFT)
		{
			if (m_ViewHovered && !Input::IsKeyPressed(BL_KEY_LEFT_ALT) && !ImGuizmo::IsOver())
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}
	void EditorLayer::OnScenePlay()
	{
		m_ActiveScene = Scene::Copy(m_EditorScene);

		m_ActiveScene->OnRuntimeStart();
		m_SceneState = SceneState::Runtime;
	}
	void EditorLayer::OnSceneStop()
	{
		m_ActiveScene->OnRuntimeStop();
		m_SceneState = SceneState::Edit;

		m_ActiveScene = m_EditorScene;
	}
	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = m_EditorScene->DuplicateEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(newEntity);
		}
	}
	void EditorLayer::UI_Toolbar()
	{
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,2 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
		auto& colors = ImGui::GetStyle().Colors;
		const auto& activeColor = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { activeColor.x, activeColor.y, activeColor.z, activeColor.w });
		const auto& hoveredColor = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { hoveredColor.x, hoveredColor.y, hoveredColor.z, hoveredColor.w });
	
		Ref<Texture2D> iconTexture = m_SceneState == SceneState::Edit ? m_PlayIcon : m_StopIcon;
		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x * 0.5f - size * 0.5f);
		if (ImGui::ImageButton((void*)(uint64_t)iconTexture->GetRendererID(), { size,size }, { 2,0 }, { 1,1 })) {
			switch (m_SceneState) 
			{
			case SceneState::Edit:
				OnScenePlay();
				break;
			case SceneState::Runtime:
				OnSceneStop();
				break;
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnSetViewport((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_FilePath = {};
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Blanco Scene(*.blanco)\0*.blanco\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}
	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState == SceneState::Runtime)
			OnSceneStop();

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer sceneSerializer(newScene);
		if (sceneSerializer.DeSerialize(path.string()))
		{
			m_EditorScene = newScene;
			m_EditorScene->OnSetViewport((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_EditorScene);

			m_ActiveScene = m_EditorScene;

			m_FilePath = path.string();
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Blanco Scene(*.blanco)\0*.blanco\0");
		if (!filepath.empty())
		{
			SceneSerializer sceneSerializer(m_ActiveScene);
			sceneSerializer.Serialize(filepath);
			m_FilePath = filepath;
		}
	}
	void EditorLayer::SaveScene()
	{
		if (!m_FilePath.empty())
		{
			SceneSerializer sceneSerializer(m_ActiveScene);
			sceneSerializer.Serialize(m_FilePath);
		}
		else
		{
			SaveSceneAs();
		}
	}
}

