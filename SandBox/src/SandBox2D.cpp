#include "SandBox2D.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"



SandBox2D::SandBox2D() :Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f),m_Tiling(10.0f)
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

		Blanco::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, m_FlatColor);
		Blanco::Renderer2D::DrawRotateQuad({ -0.8f,-0.8f }, 105.0f, { 0.5f,0.8f }, { 0.2f,0.9f,0.5f,1.0f });
		Blanco::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, m_Texture, { 0.8f,0.8f,0.8f,0.8f }, m_Tiling);


		Blanco::Renderer::EndScene();
	}
	/*auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
	std::dynamic_pointer_cast<Blanco::OpenGLShader>(flatColorShader)->Bind();
	std::dynamic_pointer_cast<Blanco::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_FlatColor", m_FlatColor);*/
}

void SandBox2D::OnEvent(Blanco::Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandBox2D::OnImguiRender()
{
	ImGui::Begin("Color test!");
	ImGui::ColorEdit4("Square Color", &m_FlatColor.r);
	ImGui::SliderFloat("cats", &m_Tiling, 1, 20);
	ImGui::End();

}
