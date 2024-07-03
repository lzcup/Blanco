#pragma once
#include <entt.hpp>
#include "Blanco/Core/TimeStep.h"

namespace Blanco
{
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");

		void OnUpdate(TimeStep ts);
		void OnSetViewport(uint32_t width, uint32_t height);
	private:
		entt::registry m_Regisrty;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}