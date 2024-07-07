#pragma once
#include "Blanco/Core/Core.h"
#include "Scene.h"

namespace Blanco
{
	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene) :m_Scene(scene) {}

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);//not implement

		bool DeSerialize(const std::string& filepath);
		bool DeSerializeRuntime(const std::string& filepath);//not implement
	private:
		Ref<Scene> m_Scene;
	};
}