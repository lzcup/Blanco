#pragma once
#include "Blanco/Renderer/FrameBuffer.h"

namespace Blanco
{
	class OpenGLFrameBuffer :public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual FrameBufferSpecification& GetSpecification() override { return m_Specification; }
		virtual const uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { return m_ColorAttachments[index]; }
		virtual const uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		void Invalidate();

	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FrameBufferTextureSpecification> m_ColorSpecification;
		FrameBufferTextureSpecification m_DepthSpecification;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}