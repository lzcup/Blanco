#pragma once
#include <Blanco.h>

class SandBox2D :public Blanco::Layer {
public:
	SandBox2D();
	virtual ~SandBox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Blanco::TimeStep ts) override;
	virtual void OnEvent(Blanco::Event& event) override;
	virtual void OnImguiRender() override;
private:
	Blanco::ShaderLibrary m_ShaderLibrary;
	Blanco::Ref<Blanco::VertexArray> m_SquaVertexArray;
	Blanco::OrthoGraphicCameraController m_CameraController;

	glm::vec4 m_FlatColor = { 0.2f, 0.3f, 0.5f ,1.0f };
	Blanco::Ref<Blanco::Texture2D> m_Texture;
	Blanco::Ref<Blanco::Texture2D> m_DoorsSpriteSheet;
	Blanco::Ref<Blanco::SubTexture2D> m_YellowDoor,m_YellowKey,m_OpenDoor;

	float m_keyx = -0.5f;
};