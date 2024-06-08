#include "BLpch.h"
#include "Renderer.h"

namespace Blanco 
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();
   
    void Renderer::BeginScene(OrthoGraphicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }
    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        vertexArray->Bind();
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->UploadUniformMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(vertexArray);
    }

}

