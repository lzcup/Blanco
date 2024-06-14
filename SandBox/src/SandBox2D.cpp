#include "SandBox2D.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"

SandBox2D::SandBox2D() :Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void SandBox2D::OnAttach()
{
	
}

void SandBox2D::OnDetach()
{
}

void SandBox2D::OnUpdate(Blanco::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Blanco::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Blanco::RenderCommand::Clear();

	Blanco::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Blanco::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, m_FlatColor, 0.0f);
	Blanco::Renderer2D::DrawQuad({ -0.8f,-0.8f }, { 0.5f,0.8f }, m_FlatColor, 105.0f);

	Blanco::Renderer::EndScene();
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
	ImGui::End();
}
