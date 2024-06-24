#include "Level.h"
#include "Random.h"
#include "Color.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"

static glm::vec3 HSVtoRGB(glm::vec3 in)
{
	double hh, p, q, t, ff;
	long i;
	glm::vec3 out = glm::vec3(0.0f);
	double h = (double)in.x * 360.0;
	double s = (double)in.y;
	double v = (double)in.z;

	if (s <= 0.0) {
		out.r = (float)v;
		out.g = (float)v;
		out.b = (float)v;
		return out;
	}
	hh = h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = v * (1.0 - s);
	q = v * (1.0 - (s * ff));
	t = v * (1.0 - (s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = (float)v;
		out.g = (float)t;
		out.b = (float)p;
		break;
	case 1:
		out.r = (float)q;
		out.g = (float)v;
		out.b = (float)p;
		break;
	case 2:
		out.r = (float)p;
		out.g = (float)v;
		out.b = (float)t;
		break;

	case 3:
		out.r = (float)p;
		out.g = (float)q;
		out.b = (float)v;
		break;
	case 4:
		out.r = (float)t;
		out.g = (float)p;
		out.b = (float)v;
		break;
	case 5:
	default:
		out.r = (float)v;
		out.g = (float)p;
		out.b = (float)q;
		break;
	}
	return out;
}

static bool PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = (pt.x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (pt.y - v2.y);
	d2 = (pt.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (pt.y - v3.y);
	d3 = (pt.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (pt.y - v1.y);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
};

Level::Level()
{
}

void Level::Init()
{
	m_Pillars.resize(5);
	for (int i = 0; i < 5; i++) {
		CreatePillar(i, i * m_Interval);
	}
	m_Player.LoadAssets();
	m_Texture = Blanco::Texture2D::Create("assets/textures/Traingle.png");
}

void Level::OnUpdate(Blanco::TimeStep ts)
{
	if (CollisionTest()) {
		m_GameOver = true;
		return;
	};

	m_Player.OnUpdate(ts);

	m_PillarHSV.x += 0.1f * ts;
	if (m_PillarHSV.x > 1.0f)
		m_PillarHSV.x = 0.0f;
	if (m_Player.GetPosition().x + m_LevelWidth / 2 >= m_PillarEdge) {
		m_PillarIndex = m_PillarIndex % m_Pillars.size();
		CreatePillar(m_PillarIndex++, ((int)((m_Player.GetPosition().x + m_LevelWidth / 2) / m_Interval) + 1) * m_Interval);
		m_PillarEdge += m_Interval;
	}
	
}

void Level::OnRender()
{
	glm::vec4 color = glm::vec4(HSVtoRGB(m_PillarHSV), 1.0f);

	//Background
	Blanco::Renderer2D::DrawQuad({ m_Player.GetPosition().x,0.0f,-0.499999f }, { 20.0f,20.0f }, Color::Grey);

	//Floor and ceiling
	Blanco::Renderer2D::DrawQuad({ m_Player.GetPosition().x,12.0f }, { 20.0f,10.0f }, color);
	Blanco::Renderer2D::DrawQuad({ m_Player.GetPosition().x,-12.0f }, { 20.0f,10.0f }, color);

	for (Pillar& pillar : m_Pillars) {
		Blanco::Renderer2D::DrawRotateQuad(pillar.TopPosition, glm::radians(180.0f), pillar.TopScale, m_Texture, color);
		Blanco::Renderer2D::DrawQuad(pillar.BottomPosition, pillar.BottomScale, m_Texture, color);
	}
	m_Player.OnRender();
}

void Level::OnImguiRender()
{
	//m_Player.OnImguiRender();
	//ImGui::Begin("pillar");
	//ImGui::SliderFloat2("TopPosition", &(m_Pillars[0].TopPosition.x), 0, 15.0f);
	//ImGui::SliderFloat2("BottomPosition", &(m_Pillars[0].BottomPosition.x), -15.0, 0);
	//ImGui::SliderFloat3("HSV", &(m_PillarHSV.x), 0.0, 1.0f);
	//ImGui::End();

}

void Level::CreatePillar(int index, float offset)
{
	Pillar& pillar = m_Pillars[index];
	pillar.TopPosition.x = offset;
	pillar.BottomPosition.x = offset;
	pillar.TopPosition.z = 0.1f * index - 0.5f+0.001f;
	pillar.BottomPosition.z = 0.1f * index - 0.5f + 0.01f;

	float center = Random::Float() * 20.0f - 10.0f;
	float gap = Random::Float() * 1.0f;

	pillar.TopPosition.y = 5.0f + center * 0.2f + gap;
	pillar.BottomPosition.y = -5.0f + center * 0.2f - gap;
}

void Level::Reset()
{
	m_GameOver = false;

	m_PillarIndex = 0;
	m_PillarEdge = 4 * m_Interval + 4.0f;
	m_PillarHSV = { 0.0f,0.8f,0.8f };
	m_Pillars.resize(5);
	for (int i = 0; i < 5; i++) {
		CreatePillar(i, i * m_Interval);
	}

	m_Player.Reset();
}

bool Level::CollisionTest()
{
	glm::vec4 playerVertices[4] = {
		{-0.5f,-0.5f,0.0f,1.0f },
		{-0.5f, 0.5f,0.0f,1.0f },
		{ 0.5f, 0.5f,0.0f,1.0f },
		{ 0.5f,-0.5f,0.0f,1.0f }
	};
	glm::vec4 palyerTransformVertices[4];
	for (int i = 0; i < 4; i++) {
		palyerTransformVertices[i] = glm::translate(glm::mat4(1.0f), { m_Player.GetPosition().x,m_Player.GetPosition().y,0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f,0.0f,1.0f })
			* playerVertices[i];
		if (palyerTransformVertices[i].y <= -7.0f || palyerTransformVertices[i].y >= 7.0f)
			return true;
	}

	

	//pillar collision check
	glm::vec4 pillarVertices[3] = {
		{-0.5f,-0.5f,0.0f,1.0f },
		{ 0.5f,-0.5f,0.0f,1.0f },
		{ 0.0f, 0.5f,0.0f,1.0f }
	};

	for (Pillar& p : m_Pillars) {
		glm::vec2 tri[3];
		for (int i = 0; i < 3; i++) {
			tri[i] = glm::translate(glm::mat4(1.0f), { p.TopPosition.x,p.TopPosition.y,0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f,0.0f,1.0f })
				* glm::scale(glm::mat4(1.0f), { p.TopScale.x,p.TopScale.y,0.0f })
				* pillarVertices[i];
		}
		for (int i = 0; i < 4; i++) {
			if(PointInTriangle({ palyerTransformVertices[i].x,palyerTransformVertices[i].y }, 
				{ tri[0].x, tri[0].y }, { tri[1].x, tri[1].y }, { tri[2].x, tri[2].y })){ 
				return true;
			}
		}
		for (int i = 0; i < 3; i++) {
			tri[i] = glm::translate(glm::mat4(1.0f), { p.BottomPosition.x,p.BottomPosition.y,0.0f })
				* glm::scale(glm::mat4(1.0f), { p.BottomScale.x,p.BottomScale.y,0.0f })
				* pillarVertices[i];
		}
		for (int i = 0; i < 4; i++) {
			if (PointInTriangle({ palyerTransformVertices[i].x,palyerTransformVertices[i].y },
				{ tri[0].x, tri[0].y }, { tri[1].x, tri[1].y }, { tri[2].x, tri[2].y })) {
				return true;
			}
		}
	}
	return false;
}
