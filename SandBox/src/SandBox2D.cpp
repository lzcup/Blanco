#include "SandBox2D.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"



SandBox2D::SandBox2D() :Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void SandBox2D::OnAttach()
{
	m_Texture = Blanco::Texture2D::Create("assets/textures/cat.png");
}

void SandBox2D::OnDetach()
{
}

void SandBox2D::OnUpdate(Blanco::TimeStep ts)
{
	BL_PROFILE_FUNCTION();

	Blanco::Renderer2D::ResetStats();
	{
		BL_PROFILE_SCOPE("CameraController Update");
		m_CameraController.OnUpdate(ts);
	}

	{
		BL_PROFILE_SCOPE("BufferClear");
		Blanco::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Blanco::RenderCommand::Clear();
	}

	{
		BL_PROFILE_SCOPE("Render Scene");
		Blanco::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Blanco::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 0.3f,0.3f }, m_FlatColor);
		Blanco::Renderer2D::DrawQuad({ 1.0f,0.0f }, { 1.0f,1.5f }, { 0.8f,0.5f,0.5f,1.0f });
		//Blanco::Renderer2D::DrawRotateQuad({ -0.8f,-0.8f }, 105.0f, { 0.5f,0.8f }, { 0.2f,0.9f,0.5f,1.0f });
		//Blanco::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, m_Texture, { 0.8f,0.8f,0.8f,0.8f }, m_Tiling);

		Blanco::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { 0.1f * x + 0.5f,0.1f * y + 0.5f,0.6f,0.7f };
				Blanco::Renderer2D::DrawQuad({ x,y }, { 0.45f,0.45f }, color);
			}
		}
		Blanco::Renderer2D::EndScene();
	}
}

void SandBox2D::OnEvent(Blanco::Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandBox2D::OnImguiRender()
{
	auto& stats = Blanco::Renderer2D::GetStats();
	ImGui::Begin("Statistics");
	ImGui::Text("DrawCalls:%d", stats.DrawCalls);
	ImGui::Text("DrawQuads:%d", stats.DrawQuads);
	ImGui::Text("DrawVertices:%d", stats.DrawVertices);
	ImGui::Text("DrawIndices:%d", stats.DrawIndices);
	ImGui::ColorEdit4("Square Color", &m_FlatColor.r);
	ImGui::End();

}
