#pragma once
#include <Blanco.h>
#include "Panel/SceneHierarchyPanel.h"
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
		void NewFile();
		void OpenFileAs();
		void SaveFileAs();
		void SaveFile();
	private:
		OrthoGraphicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec4 m_FlatColor = { 0.2f, 0.3f, 0.5f ,1.0f };
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_DoorsSpriteSheet;
		Ref<SubTexture2D> m_YellowDoor, m_YellowKey, m_OpenDoor;
		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_PrimaryCamera;
		Entity m_SecondCamera;
		SceneHierarchyPanel m_SceneHierarchyPanel;

		EditorCamera m_EditorCamera;

		glm::vec2 m_Viewport = { 1280.0f,720.0f };
		bool m_ViewFocuse = false;
		bool m_ViewHovered = false;

		std::string m_FilePath = {};
		int m_GuizmoOperation = -1;
	};
}
