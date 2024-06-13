#include "BLpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Blanco 
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();
   
    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::OnWindowResize(float x, float y, float width, float height)
    {
        RenderCommand::SetViewport(x, y, width, height);
    }

    void Renderer::BeginScene(OrthoGraphicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }
    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        vertexArray->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(vertexArray);
    }

}

