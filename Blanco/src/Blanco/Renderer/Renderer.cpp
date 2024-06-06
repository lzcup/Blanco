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

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->Bind();
        vertexArray->Bind();
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        RenderCommand::DrawIndexed(vertexArray);
    }

}

