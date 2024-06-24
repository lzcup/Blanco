#include "BLpch.h"
#include "OpenGLBuffer.h"
#include "glad/gl.h"

namespace Blanco
{
	//vertex////////////////////////////////////////
	///////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		BL_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		BL_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		BL_PROFILE_FUNCTION();

		glDeleteBuffers(GL_ARRAY_BUFFER, &m_RendererID);
	}

	void OpenGLVertexBuffer::SetData(uint32_t size, void* data)
	{
		BL_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		BL_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		BL_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//index////////////////////////////////////////
	///////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) :m_Count(count)
	{
		BL_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		BL_PROFILE_FUNCTION();

		glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		BL_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		BL_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

