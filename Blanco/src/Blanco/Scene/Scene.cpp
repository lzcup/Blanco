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
		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		return entity;
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		SceneCamera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Regisrty.group<CameraComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);
				if (camera.Primary) {
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}
		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Regisrty.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
		
	}
	void Scene::OnSetViewport(uint32_t width, uint32_t height)
	{
		auto view = m_Regisrty.view<CameraComponent>();
		for (auto entity : view) {
			CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}
}

