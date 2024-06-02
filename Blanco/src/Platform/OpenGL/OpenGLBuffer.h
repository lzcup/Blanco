#pragma once
#include "Blanco/Renderer/Buffer.h"

namespace Blanco
{
	class OpenGLVertexBuffer :public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		void Bind() const override;
		void UnBind() const override;
	private:
		uint32_t m_RendererID;
	};

	class OpenGLIndexBuffer :public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		inline uint32_t GetCount() const{ return m_Count; }

		void Bind() const override;
		void UnBind() const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}