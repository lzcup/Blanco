#pragma once
#include "Blanco/Core/Core.h"

namespace Blanco 
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,

		//Depth
		DEPTH24STENCIL8,

		//default
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format):TextureFormat(format){}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
		//filter and so on
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			:Attachments(attachments) {}

	    std::vector<FrameBufferTextureSpecification> Attachments;
	};


	struct FrameBufferSpecification{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;
		FrameBufferAttachmentSpecification Attachments;
		bool SwapChainTarget = false;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() {}

		virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual const uint32_t GetDepthAttachmentRendererID() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}
	