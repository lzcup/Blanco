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
		void DestoryEntity(Entity& entity);

		void OnUpdate(TimeStep ts);
		void OnSetViewport(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity,T& component);
	private:
		entt::registry m_Regisrty;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}