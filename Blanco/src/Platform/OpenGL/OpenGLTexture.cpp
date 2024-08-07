#include "BLpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include "glad/gl.h"

namespace Blanco
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) :
		m_Width(width), m_Height(height), m_Internalformat(GL_RGBA8), m_Dataformat(GL_RGBA)
	{
		BL_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_Internalformat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :m_Path(path)
	{
		BL_PROFILE_FUNCTION();

		stbi_set_flip_vertically_on_load(1);

		int width, height,channels;
		stbi_uc* data;
		{
			BL_PROFILE_SCOPE("LoadFile-OpenGLTexture2D(const std::string& path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		BL_CORE_ASSERT(data,"Unable to load image")
		m_Width = width;
		m_Height = height;

		if (channels == 4) {
			m_Internalformat = GL_RGBA8;
			m_Dataformat = GL_RGBA;
		}
		else if (channels == 3) {
			m_Internalformat = GL_RGB8;
			m_Dataformat = GL_RGB;
		}
		BL_CORE_ASSERT((m_Internalformat & m_Dataformat), "ImageFormat is not supported")

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_Internalformat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Dataformat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		BL_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		BL_PROFILE_FUNCTION();

		uint32_t bpp = m_Dataformat == GL_RGBA ? 4 : 3;
		BL_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire size!")
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Dataformat, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		BL_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
	void OpenGLTexture2D::UnBind(uint32_t slot)
	{
		BL_PROFILE_FUNCTION();

		glBindTextureUnit(slot, 0);
	}
}