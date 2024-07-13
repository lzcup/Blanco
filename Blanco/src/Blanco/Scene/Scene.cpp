#include "BLpch.h"
#include "Scene.h"
#include <glm.hpp>
#include "Components.h"
#include "Blanco/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Blanco
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Regisrty.create(), this);
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestoryEntity(Entity& entity)
	{
		m_Regisrty.destroy(entity);
	}

	void Scene::OnRuntimeUpdate(TimeStep ts)
	{
		//TODO remove to scene play
		m_Regisrty.view<NativeScriptComponent>().each([=](auto entity,auto& nsc) {
			if (!nsc.Instance) {
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity(entity, this);
				nsc.Instance->OnCreate();
			}
			nsc.Instance->OnUpdate(ts);
			});

		SceneCamera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = m_Regisrty.group<CameraComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);
				if (camera.Primary) {
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}
		if (mainCamera) {
			auto group = m_Regisrty.group<TransformComponent>(entt::get<SpriteComponent>);
			if (group.size()) {
				Renderer2D::BeginScene(*mainCamera, cameraTransform);

				for (auto entity : group) {
					auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
					Renderer2D::DrawSprite(transform.GetTransform(), sprite,(int)entity);
				}

				Renderer2D::EndScene();
			}
		}
		
	}

	void Scene::OnEditorUpdate(TimeStep ts, EditorCamera& camera)
	{
		auto group = m_Regisrty.group<TransformComponent>(entt::get<SpriteComponent>);
		if (group.size()) {
			Renderer2D::BeginScene(camera);

			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite,(int)entity);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnSetViewport(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		auto view = m_Regisrty.view<CameraComponent>();
		for (auto entity : view) {
			CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCamera()
	{
		auto view = m_Regisrty.view<CameraComponent>();
		for (auto entity : view) {
			CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.Primary) {
				return Entity(entity, this);
			}
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity& entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded(Entity& entity, TagComponent& component){}

	template<>
	void Scene::OnComponentAdded(Entity& entity, TransformComponent& component) {}

	template<>
	void Scene::OnComponentAdded(Entity& entity, SpriteComponent& component) {}

	template<>
	void Scene::OnComponentAdded(Entity& entity, CameraComponent& component) {
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded(Entity& entity, NativeScriptComponent& component) {}
}

