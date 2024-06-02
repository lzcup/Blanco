#pragma once

namespace Blanco
{
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {};

		static VertexBuffer* CreatVertextBuffer(float* vertices, uint32_t size);

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {};

		static IndexBuffer* CreatIndexBuffer(uint32_t* indices, uint32_t count);

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}