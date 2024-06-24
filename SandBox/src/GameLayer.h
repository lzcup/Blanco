#pragma once
#include <Blanco.h>
#include "Level.h"
#include "imgui.h"

enum class GameState
{
	Play = 0,
	MainMenu,
	GameOver
};

class GameLayer :public Blanco::Layer {
public:
	GameLayer();
	virtual ~GameLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Blanco::TimeStep ts) override;
	virtual void OnEvent(Blanco::Event& event) override;
	virtual void OnImguiRender() override;
private:
	void CreateCamera(uint32_t width, uint32_t height);
	bool OnMouseButtonPressed(Blanco::MouseButtonPressedEvent& e);
private:
	Blanco::Ref<Blanco::VertexArray> m_SquaVertexArray;
	Blanco::Scope<Blanco::OrthoGraphicCamera> m_Camera;

	Level m_Level;
	uint32_t m_Width, m_Height;

	GameState m_State;
	ImFont* m_Font;
};