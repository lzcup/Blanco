#pragma once
#include "Blanco/Renderer/Texture.h"

namespace Blanco
{
	class OpenGLTexture2D :public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D() override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual void Bind(uint32_t slot) override;
	private:
		uint32_t m_RendererId;
		uint32_t m_Width, m_Height;
	};
}