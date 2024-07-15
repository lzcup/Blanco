#pragma once
#include <Blanco.h>
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ContentBrowserPanel.h"
#include "Blanco/Renderer/EditorCamera.h"

namespace Blanco
{
	class EditorLayer :public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImguiRender() override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void OnScenePlay();
		void OnSceneStop();
	private:
		void UI_Toolbar();
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void SaveScene();
	private:
		OrthoGraphicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec4 m_FlatColor = { 0.2f, 0.3f, 0.5f ,1.0f };
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_PlayIcon, m_StopIcon;
		Ref<Texture2D> m_DoorsSpriteSheet;
		Ref<SubTexture2D> m_YellowDoor, m_YellowKey, m_OpenDoor;
		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_PrimaryCamera;
		Entity m_SecondCamera;
		EditorCamera m_EditorCamera;

		Entity m_HoveredEntity = {};

		//Panel
		enum class SceneState
		{
			Edit = 0, Runtime = 1
		};

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		SceneState m_SceneState = SceneState::Edit;

		bool m_ViewFocuse = false;
		bool m_ViewHovered = false;
		glm::vec2 m_ViewportMinBound = { 0.0f,0.0f }; //viewport upper left corner
		glm::vec2 m_ViewportSize = { 1280.0f,720.0f };

		std::string m_FilePath = {};
		int m_GuizmoOperation = -1;
	};
}
