#pragma once
#include "Blanco.h"
#include <glm.hpp>
#include "Player.h"

struct Pillar {
	glm::vec3 TopPosition;
	glm::vec2 TopScale = { 8.0f,8.0f };

	glm::vec3 BottomPosition;
	glm::vec2 BottomScale{ 8.0f,8.0f };
};

class Level {
public:
	Level();
	~Level() {};
	void Init();
	void OnUpdate(Blanco::TimeStep ts);
	void OnRender();
	void OnImguiRender();
	void CreatePillar(int index, float offset);
	void Reset();

	bool CollisionTest();
	const Player& GetPlayer() const { return m_Player; }
	const float GetLevelWidth() const { return m_LevelWidth; }
	const bool IsGameOver() const { return m_GameOver; }
public:
	Blanco::Ref<Blanco::Texture2D> m_Texture;
	Player m_Player;
	float m_LevelWidth = 20.0f;
	float m_Interval = 7.0f;

	std::vector<Pillar> m_Pillars;
	int m_PillarIndex = 0;
	float m_PillarEdge = 4 * m_Interval + 4.0f;
	glm::vec3 m_PillarHSV = { 0.0f,0.8f,0.8f };

	bool m_GameOver = false;
};