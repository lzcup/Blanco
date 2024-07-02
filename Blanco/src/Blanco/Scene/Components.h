#pragma once
#include <glm.hpp>
#include "SceneCamera.h"

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
		glm::mat4 Transform{ 1.0f };
		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform) :Transform(transform) {}
		TransformComponent(const TransformComponent&) = default;

		operator const glm::mat4& () const { return Transform; }
		operator glm::mat4& () { return Transform; }
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
}