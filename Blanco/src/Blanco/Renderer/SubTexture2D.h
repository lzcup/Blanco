#pragma once
#include "Texture.h"
#include <glm.hpp>

namespace Blanco
{
	class SubTexture2D {
	public:
		SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max);

		Ref<Texture2D> GetTexture() const{ return m_Texture; }
		const glm::vec4& GetCoords() const { return m_Coords; }

		static Ref<SubTexture2D> CreateTextureByCoords(Ref<Texture2D> texture, const glm::vec2& postion, const glm::vec2& cellSize, const glm::vec2& targetCells);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec4 m_Coords;
	};
}