#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "Blanco/Scene/Components.h"


namespace Blanco
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("SceneHierarchyPanel");

		m_Context->m_Regisrty.view<TagComponent>().each([&](auto entityID,auto& tc) {
			Entity entity(entityID, m_Context.get());
			DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && (!ImGui::IsAnyItemHovered()))
			m_SelectionContext = {};

		ImGui::End();

		if (m_SelectionContext) {
			ImGui::Begin("Properties");
			DrawProperties(m_SelectionContext);
			ImGui::End();
		}
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entity.GetComponent<TagComponent>().Tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}
		if (opened) {
			ImGui::TreePop();
		}
	}
	void SceneHierarchyPanel::DrawProperties(Entity& entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("tag", buffer, sizeof(buffer))) {
				tag = buffer;
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("transform", &transform[3][0], 0.1f);
				ImGui::TreePop();
			}
		}
	}
}

