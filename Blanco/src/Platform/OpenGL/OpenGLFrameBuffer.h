#pragma once
#include "Blanco/Renderer/FrameBuffer.h"

namespace Blanco
{
	class OpenGLFrameBuffer :public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual FrameBufferSpecification& GetSpecification() override { return m_Specification; }
		virtual const uint32_t GetColorAttchmentRendererID() const override { return m_ColorAttachment; }
		virtual const uint32_t GetDepthAttchmentRendererID() const override { return m_DepthAttachment; }

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		void Invalidate();

	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;

	};
}