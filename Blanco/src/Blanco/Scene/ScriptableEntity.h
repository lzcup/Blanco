#pragma once
#include "Entity.h"

namespace Blanco
{
	class ScriptableEntity :public Entity {
	public:
		virtual ~ScriptableEntity() {};
		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnDestory() {}
	private:
		Entity m_Entity;

		friend class Scene;
	};
}