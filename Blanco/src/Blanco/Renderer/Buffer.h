#pragma once

namespace Blanco
{

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		    case ShaderDataType::Float:    return 4;
		    case ShaderDataType::Float2:   return 4 * 2;
		    case ShaderDataType::Float3:   return 4 * 3;
		    case ShaderDataType::Float4:   return 4 * 4;
		    case ShaderDataType::Mat3:     return 4 * 3 * 3;
		    case ShaderDataType::Mat4:     return 4 * 4 * 3;
		    case ShaderDataType::Int:      return 4;
		    case ShaderDataType::Int2:     return 4 * 2;
		    case ShaderDataType::Int3:     return 4 * 3;
		    case ShaderDataType::Int4:     return 4 * 4;
		    case ShaderDataType::Bool:     return 1;
		}
		BL_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	};

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint64_t Offset;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized)
			:Type(type), Name(name), Size(ShaderDataTypeSize(Type)), Offset(0), Normalized(normalized) {
		}

		inline uint8_t GetComponentSize() const{
			switch (Type) {
			    case ShaderDataType::Float:    return 1;
			    case ShaderDataType::Float2:   return 2;
			    case ShaderDataType::Float3:   return 3;
			    case ShaderDataType::Float4:   return 4;
			    case ShaderDataType::Mat3:     return 3 * 3;
			    case ShaderDataType::Mat4:     return 4 * 3;
			    case ShaderDataType::Int:      return 1;
			    case ShaderDataType::Int2:     return 2;
			    case ShaderDataType::Int3:     return 3;
			    case ShaderDataType::Int4:     return 4;
			    case ShaderDataType::Bool:     return 1;
			}
			BL_CORE_ASSERT(false, "Unknown ShaderDataType!")
			return 0;
		}
         
	};

	class BufferLayout {
	public:
		BufferLayout() :m_Stride(0) {}

		BufferLayout(const std::initializer_list<BufferElement>& elements) :m_Elements(elements), m_Stride(0) {
			uint64_t offset = 0;
			for (BufferElement& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		};

		inline void SetElements(std::vector<BufferElement> layout) { m_Elements = layout; }
		inline std::vector<BufferElement> GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }
		inline std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {};

		static Ref<VertexBuffer> CreatVertextBuffer(uint32_t size);
		static Ref<VertexBuffer> CreatVertextBuffer(float* vertices, uint32_t size);

		virtual BufferLayout GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(uint32_t size, void* data) = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {};

		static Ref<IndexBuffer> CreatIndexBuffer(uint32_t* indices, uint32_t count);

		virtual inline uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}