#pragma once
#include <glm.hpp>
#include "SceneCamera.h"
#include "Blanco/Core/UUID.h"

#include "Blanco/Renderer/Texture.h"
#include <gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

namespace Blanco
{
	struct IDComponent
	{
		UUID ID;
		IDComponent() = default;
		IDComponent(const UUID& id) :ID(id) {}
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const std::string& tag) :Tag(tag) {}
		TagComponent(const TagComponent&) = default;
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f,0.0f,0.0f };
		glm::vec3 Rotation{ 0.0f,0.0f,0.0f };
		glm::vec3 Scale{ 1.0f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation) :Translation(translation) {}
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteComponent
	{
		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) :Color(color) {}
		SpriteComponent(const SpriteComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class ScriptableEntity;
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)() = nullptr;
		void(*DestoryScript)(NativeScriptComponent*) = nullptr;

		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T); };
			DestoryScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; };
		}
	};

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}