#include "BLpch.h"
#include "SubTexture2D.h"

namespace Blanco
{
	SubTexture2D::SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max) :m_Texture(texture){
		m_Coords.x = min.x / texture->GetWidth();
		m_Coords.y = min.y / texture->GetHeight();
		m_Coords.z = max.x / texture->GetWidth();
		m_Coords.w = max.y / texture->GetHeight();
	}
	Ref<SubTexture2D> SubTexture2D::CreateTextureByCoords(Ref<Texture2D> texture, const glm::vec2& postion, const glm::vec2& cellSize, const glm::vec2& targetCells)
	{
		glm::vec2 min = { postion.x * cellSize.x,postion.y * cellSize.y };
		glm::vec2 max = { (postion.x + targetCells.x) * cellSize.x ,(postion.y + targetCells.y) * cellSize.y };
		return CreateRef<SubTexture2D>(texture, min, max);
	}
}
