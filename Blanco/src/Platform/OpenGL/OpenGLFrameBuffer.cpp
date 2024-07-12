#include "BLpch.h"
#include "OpenGLFrameBuffer.h"
#include <glad/gl.h>

namespace Blanco
{
	namespace Utils 
	{
		static GLenum TextureTarget(bool multisample)
		{
			return multisample == true ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}
			return false;
		}

		static void CreateTextures(bool multiply, uint32_t count, uint32_t* idArray)
		{
			glCreateTextures(TextureTarget(multiply), count, idArray);
		}

		static void BindTexture(bool multiply, uint32_t id)
		{
			glBindTexture(TextureTarget(multiply), id);
		}

		static GLenum BLFrameBufferTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case Blanco::FrameBufferTextureFormat::RGBA8:
				return GL_RGBA;
			case Blanco::FrameBufferTextureFormat::RED_INTEGER:
				return GL_RED_INTEGER;
			}
			BL_CORE_ASSERT(false, "Invalidate format");
			return 0;
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalformat, GLenum format, uint32_t width, uint32_t height, uint32_t index)
		{
			bool multisample = samples > 1;
			if (multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisample), id, 0);

		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisample = samples > 1;
			if (multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisample), id, 0);
		}
	}


	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec):m_Specification(spec)
	{

		for (auto& textureSpec : spec.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(textureSpec.TextureFormat))
				m_ColorSpecification.emplace_back(textureSpec);
			else
				m_DepthSpecification = textureSpec;
		}

		Invalidate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}
	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}
	void OpenGLFrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}
	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		BL_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "max index:"+std::to_string((m_ColorAttachments.size()-1)));
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int data;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);

		return data;
	}
	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		BL_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "max index:" + std::to_string((m_ColorAttachments.size() - 1)));
		auto& spec = m_ColorSpecification[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, 
			Utils::BLFrameBufferTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multiply = m_Specification.Samples > 1;
		if (m_ColorSpecification.size())
		{
			m_ColorAttachments.resize(m_ColorSpecification.size());
			Utils::CreateTextures(multiply, (GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());

			//attachments
			for (int i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multiply, m_ColorAttachments[i]);
				switch (m_ColorSpecification[i].TextureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}
		if (m_DepthSpecification.TextureFormat != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(multiply, 1, &m_DepthAttachment);
			Utils::BindTexture(multiply, m_DepthAttachment);
			switch (m_DepthSpecification.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			BL_CORE_ASSERT(m_ColorAttachments.size() <= 4, "color attachments max 4");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
		}
		else if(m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		BL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER)== GL_FRAMEBUFFER_COMPLETE, "FrameBuffer not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
