#include "Player.h"
#include <imgui.h>

Player::Player():m_Position(-8.0f, 0.0f), m_EnginePower(4.5f), m_Gravity(4.0f), m_Rotation(270.0f), m_RotateSpeed(10.0f), m_Speed(3.0f)
{
}

void Player::LoadAssets()
{
	m_Texture = Blanco::Texture2D::Create("assets/textures/J.png");
}

void Player::OnUpdate(Blanco::TimeStep ts)
{
	if (Blanco::Input::IsKeyPressed(BL_KEY_SPACE)) {
		m_Position.y += m_EnginePower * ts;
		m_Rotation += 120.0f * ts;
		m_Rotation = std::min(m_Rotation, 320.0f);
	}
	else {
		m_Position.y -= m_Gravity * ts;
		m_Rotation -= 80.0f * ts;
		m_Rotation = std::max(m_Rotation, 180.0f);
	}
	if (Blanco::Input::IsKeyPressed(BL_KEY_RIGHT)) {
		m_Position.x += m_Speed * ts;
	}
}

void Player::OnRender()
{
	Blanco::Renderer2D::DrawRotateQuad(glm::vec3(m_Position, 0.1f), glm::radians(m_Rotation), { 1.0f,1.0f }, m_Texture);
}

void Player::OnImguiRender()
{
	/*ImGui::Begin("Player Property");
	ImGui::SliderFloat("EnginePower", &m_EnginePower, 0, 10.0f);
	ImGui::SliderFloat("Gravity", &m_Gravity, 0, 10.0f);
	ImGui::SliderFloat("Rotation", &m_Rotation, 0, 360.0f);
	ImGui::SliderFloat("Speed", &m_Speed, 0, 10.0f);
	ImGui::SliderFloat2("Position", &m_Position.x, 0, 60.0f);
	ImGui::End();*/
}

void Player::Reset()
{
	m_Position = { -8.0f, 0.0f };
	m_Rotation = 270.0f;
}


