#include "GameLayer.h"
#include "gtc/matrix_transform.hpp"


GameLayer::GameLayer() :Layer("GameLayer")
{
	auto& window = Blanco::Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());
}

void GameLayer::OnAttach()
{
	m_State = GameState::MainMenu;
	m_Level.Init();

	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/keai.ttf",120.0f);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Blanco::TimeStep ts)
{
	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;
	
	Blanco::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Blanco::RenderCommand::Clear();

	if (m_State == GameState::Play) {
		m_Level.OnUpdate(ts);
	}

	m_Camera->SetPosition(glm::vec3(m_Level.GetPlayer().GetPosition().x, m_Level.GetPlayer().GetPosition().y, 0));
	Blanco::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();

	Blanco::Renderer::EndScene();
}

void GameLayer::OnEvent(Blanco::Event& e)
{
	if (m_State == GameState::MainMenu || m_State == GameState::GameOver) {
		Blanco::Dispatcher dispatcher(e);
		dispatcher.Dispatch<Blanco::MouseButtonPressedEvent>(BL_BIND_EVENT_FNC(GameLayer::OnMouseButtonPressed));
	}
}

void GameLayer::OnImguiRender()
{
	m_Level.OnImguiRender();
	switch (m_State)
	{
	case GameState::Play:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Blanco::Application::Get().GetWindow().GetWidth();
		auto height = Blanco::Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 1100.0f;
		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		std::string scoreStr = "Your Score:" + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 50.0f, pos, 0xffffffff, scoreStr.c_str());
		break;
	}
	case GameState::MainMenu:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Blanco::Application::Get().GetWindow().GetWidth();
		auto height = Blanco::Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 900.0f;
		pos.y += 50.0f;
		ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to play!");
		break;
	}
	case GameState::GameOver:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Blanco::Application::Get().GetWindow().GetWidth();
		auto height = Blanco::Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 900.0f;
		pos.y += 50.0f;
 		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		std::string scoreStr = "GG Your Score:" + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 100.0f, pos, 0xffffffff, scoreStr.c_str());
		pos.x += 200.0f;
		pos.y += 150.0f;
		ImGui::GetForegroundDrawList()->AddText(m_Font, 50.0f, pos, 0xffffffff, "Click to continue!");
		break;
	}
	}
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{ 
	float aspecRadio = (float)width / (float)height;
	float camWidth = m_Level.GetLevelWidth() / 2;
	float bottom = -camWidth / aspecRadio;
	float top = camWidth / aspecRadio;
	float left = -camWidth;
	float right = camWidth;
	m_Camera = Blanco::CreateScope<Blanco::OrthoGraphicCamera>(left, right, bottom, top);
}

bool GameLayer::OnMouseButtonPressed(Blanco::MouseButtonPressedEvent& e)
{
	m_Level.Reset();
	m_State = GameState::Play;
	return false;
}
