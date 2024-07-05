#pragma once
#include <glm.hpp>
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include <gtc/matrix_transform.hpp>

namespace Blanco
{
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
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 }) *
				glm::rotate(glm::mat4(1.0f), Rotation.y, { 0,1,0 }) *
				glm::rotate(glm::mat4(1.0f), Rotation.z, { 0,0,1 });
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteComponent
	{
		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
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
}