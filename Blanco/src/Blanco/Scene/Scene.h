#pragma once
#include <entt.hpp>
#include "Blanco/Core/TimeStep.h"
#include "Blanco/Renderer/EditorCamera.h"

class b2World;

namespace Blanco
{
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DestoryEntity(Entity& entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnRuntimeUpdate(TimeStep ts);
		void OnEditorUpdate(TimeStep ts, EditorCamera& camera);
		void OnSetViewport(uint32_t width, uint32_t height);

		Entity GetPrimaryCamera();
	private:
		template<typename T>
		void OnComponentAdded(Entity& entity,T& component);
	private:
		entt::registry m_Regisrty;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}