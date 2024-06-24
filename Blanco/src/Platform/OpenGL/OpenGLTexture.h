#pragma once
#include "Blanco/Renderer/Texture.h"
#include "glad/gl.h"

namespace Blanco
{
	class OpenGLTexture2D :public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D() override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) override;
		virtual void UnBind(uint32_t slot) override;
	private:
		uint32_t m_RendererId;
		uint32_t m_Width, m_Height;
		GLenum m_Internalformat = 0;
		GLenum m_Dataformat = 0;
	};
}