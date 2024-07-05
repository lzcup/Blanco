#pragma once
#include "Blanco/Scene/Scene.h"
#include "Blanco/Core/Core.h"
#include "Blanco/Core/Log.h"
#include "Blanco/Scene/Entity.h"

namespace Blanco
{
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImguiRender();
		void DrawEntityNode(Entity& entity);
		void DrawProperties(Entity& entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}