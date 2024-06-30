#pragma once
#include <Blanco.h>

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
		OrthoGraphicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec4 m_FlatColor = { 0.2f, 0.3f, 0.5f ,1.0f };
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_DoorsSpriteSheet;
		Ref<SubTexture2D> m_YellowDoor, m_YellowKey, m_OpenDoor;
		float m_Rotate = 0.0f;

		glm::vec2 m_Viewport = { 0,0 };
		bool m_ViewFocuse = false;
		bool m_ViewHovered = false;
	};
}
