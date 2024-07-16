#include "BLpch.h"
#include "Scene.h"
#include <glm.hpp>
#include "Components.h"
#include "Blanco/Renderer/Renderer2D.h"
#include "Entity.h"
#include <Box2D/b2_world.h>
#include <Box2D/b2_body.h>
#include <Box2D/b2_polygon_shape.h>
#include <Box2D/b2_fixture.h>

namespace Blanco
{
	static b2BodyType BLRigidbody2DBodyTypeTob2BodyType(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:
			return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic:
			return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic:
			return b2_kinematicBody;
		}
		BL_CORE_ASSERT(false, "Unkonw BodyType");
		return b2_staticBody;
	}

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

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_Regisrty.view<Rigidbody2DComponent>();
		for (auto entityID : view) 
		{
			Entity entity = Entity(entityID, this);
			auto& transform= entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		
			b2BodyDef bodyDef;
			bodyDef.type = BLRigidbody2DBodyTypeTob2BodyType(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* b2body = m_PhysicsWorld->CreateBody(&bodyDef);
			b2body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = b2body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(transform.Scale.x * bc2d.Size.x, transform.Scale.y * bc2d.Size.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;

				b2Fixture* b2fixture = b2body->CreateFixture(&fixtureDef);
				bc2d.RuntimeFixture = b2fixture;
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
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

		//physic2d
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

		auto view = m_Regisrty.view<Rigidbody2DComponent>();
		for (auto entityID : view)
		{
			Entity entity = Entity(entityID, this);
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			transform.Translation.x = body->GetPosition().x;
			transform.Translation.y = body->GetPosition().y;
			transform.Rotation.z = body->GetAngle();
		}

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

	template<>
	void Scene::OnComponentAdded(Entity& entity, Rigidbody2DComponent& component) {}

	template<>
	void Scene::OnComponentAdded(Entity& entity, BoxCollider2DComponent& component) {}
}

