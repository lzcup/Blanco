#pragma once
#include "Blanco.h"
#include <glm.hpp>

class Player {
public:
	Player();
	~Player() {};
	void LoadAssets();
	void OnUpdate(Blanco::TimeStep ts);
	void OnRender();
	void OnImguiRender();
	void Reset();
	const glm::vec2& GetPosition() const { return m_Position; }
	float GetRotation() const { return m_Rotation; }
	int GetScore() const { return m_Position.x >= 0 ? (int)(m_Position.x / 7.0f) + 1 : 0; }
private:
	float m_EnginePower;
	float m_Gravity;
	float m_Speed;
	float m_Rotation;
	float m_RotateSpeed;
	glm::vec2 m_Position;
	Blanco::Ref<Blanco::Texture2D> m_Texture;
};