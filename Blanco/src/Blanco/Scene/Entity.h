#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace Blanco
{
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene):m_EntityHandle(entity),m_Scene(scene){}
		~Entity() {};

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Regisrty.all_of<T>(m_EntityHandle);
		}
		template<typename T,typename... Args>
		T& AddComponent(Args&&... args) {
			BL_ASSERT(!HasComponent<T>(), "This component already exist!");
			return m_Scene->m_Regisrty.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}
		template<typename T>
		T& GetComponent() {
			BL_ASSERT(HasComponent<T>(), "Entity do not have this component!");
			return m_Scene->m_Regisrty.get<T>(m_EntityHandle);
		}
		template<typename T>
		void RemoveComponent() {
			BL_ASSERT(HasComponent<T>(), "Entity do not have this component!");
			m_Scene->m_Regisrty.remove<T>(m_EntityHandle);
		}

		operator bool() const{ return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		bool operator==(const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};
}