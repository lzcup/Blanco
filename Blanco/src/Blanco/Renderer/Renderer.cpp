#include "BLpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Blanco 
{
    Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();
   
    void Renderer::Init()
    {
        BL_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::OnWindowResize(float x, float y, float width, float height)
    {
        RenderCommand::SetViewport(x, y, width, height);
    }

    void Renderer::BeginScene(OrthoGraphicCamera& camera)
    {
        BL_PROFILE_FUNCTION();

        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }
    void Renderer::EndScene()
    {
        BL_PROFILE_FUNCTION();
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
		BL_PROFILE_FUNCTION();

        shader->Bind();
        vertexArray->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(vertexArray);
    }

}

