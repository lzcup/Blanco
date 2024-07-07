#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "imgui_internal.h"
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
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("SceneHierarchyPanel");

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight|ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");
			ImGui::EndPopup();
		}

		m_Context->m_Regisrty.view<TagComponent>().each([&](auto entityID, auto& tc) {
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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entity.GetComponent<TagComponent>().Tag.c_str());
		bool removeEntity = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Remove Entity"))
				removeEntity = true;
			ImGui::EndPopup();
		}
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}
		if (opened) {
			ImGui::TreePop();
		}
		if (removeEntity) {
			m_Context->DestoryEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Controller(const std::string& lable, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGui::PushID(lable.c_str());
		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[0];
		
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", lable.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f,0.3f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f,0.2f,0.2f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f,0.8f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f,0.9f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f,0.8f,0.2f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f,0.1f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f,0.2f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f,0.1f,0.8f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template <typename T, typename Func>
	static void DrawComponent(const char* name, Entity& entity, Func function)
	{
		const ImGuiTreeNodeFlags imguiTreeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (entity.HasComponent<T>()) {
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), imguiTreeFlag, name);
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove Component")) {
					removeComponent = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (opened) {
				function(component);
				ImGui::TreePop();
			}
			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawProperties(Entity& entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##tag", buffer, sizeof(buffer))) {
				tag = buffer;
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent")) {
			if (!m_SelectionContext.HasComponent<CameraComponent>()) {
				if (ImGui::MenuItem("Camera")) {
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_SelectionContext.HasComponent<SpriteComponent>()) {
				if (ImGui::MenuItem("Sprite Renderer")) {
					m_SelectionContext.AddComponent<SpriteComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Controller("Translation", component.Translation);
			auto rotation = glm::degrees(component.Rotation);
			DrawVec3Controller("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Controller("Scale", component.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			SceneCamera& camera = component.Camera;
			const char* projectionTypes[] = { "Perspective","Orthographic" };
			SceneCamera::ProjectionType currentProjectionType = camera.GetProjectionType();
			if (ImGui::BeginCombo("ProjectionType", projectionTypes[currentProjectionType])) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionType == i;
					if (ImGui::Selectable(projectionTypes[i], isSelected)) {
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}
				}
				ImGui::EndCombo();
			}
			if (currentProjectionType == SceneCamera::ProjectionType::Perspective) {
				float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFov());
				if (ImGui::DragFloat("Fov", &verticalFov))
					camera.SetPerspectiveVerticalFov(glm::radians(verticalFov));

				float perspectiveNear = camera.GetPerspectiveNear();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNear(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFar();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFar(perspectiveFar);
			}
			if (currentProjectionType == SceneCamera::ProjectionType::Orthographic) {
				float size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &size))
					camera.SetOrthographicSize(size);

				float nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("NearClip", &nearClip))
					camera.SetOrthographicNearClip(nearClip);

				float farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("FarClip", &farClip))
					camera.SetOrthographicFarClip(farClip);

				ImGui::Checkbox("FixedAspectRatio", &(component.FixedAspectRatio));
			}
			ImGui::Checkbox("Primary Camera", &(component.Primary));
			});

		DrawComponent<SpriteComponent>("Sprite", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", &component.Color.r);
			});

	}



}

