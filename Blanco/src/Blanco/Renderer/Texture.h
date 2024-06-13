#pragma once
#include "Blanco/Core/Core.h"
#include <stdint.h>
#include <string>

namespace Blanco
{
	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) = 0;
	};

	class Texture2D :public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}