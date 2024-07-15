#include "BLpch.h"
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include "Components.h"

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Blanco
{
	static YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	static YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out,Entity& entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "123456789";//TODO: Entity ID Here

		auto& tag = entity.GetComponent<TagComponent>();
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; //TagComponent
		out << YAML::Key << "Tag" << YAML::Value << tag.Tag;
		out << YAML::EndMap;  //TagComponent
		if (entity.HasComponent<TransformComponent>()) {
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;  //TransformComponent
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;  //TransformComponent
		}
		if (entity.HasComponent<SpriteComponent>()) {
			auto& sc = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;  //SpriteComponent
			out << YAML::Key << "Color" << YAML::Value << sc.Color;
			if (sc.Texture)
				out << YAML::Key << "Texture" << YAML::Value << sc.Texture->GetTexturePath();
			out << YAML::Key << "TilingFactor" << YAML::Value << sc.TilingFactor;
			out << YAML::EndMap;  //SpriteComponent
		}
		if (entity.HasComponent<CameraComponent>()) {
			auto& cc = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;  //CameraComponent
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)cc.Camera.GetProjectionType();
			out << YAML::Key << "PerspectiveVerticalFov" << YAML::Value << cc.Camera.GetPerspectiveVerticalFov();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << cc.Camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << cc.Camera.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << cc.Camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNearClip" << YAML::Value << cc.Camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFarClip" << YAML::Value << cc.Camera.GetOrthographicFarClip();
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;
			out << YAML::EndMap;  //CameraComponent
		}
		if (entity.HasComponent<NativeScriptComponent>()) {
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap;  //NativeScriptComponent
			out << YAML::Key << "NativeScript" << YAML::Value << true;
			out << YAML::EndMap;  //NativeScriptComponent
		}
		out << YAML::EndMap;
		
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Name";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Regisrty.view<TagComponent>().each([&](auto entityID, auto& tag) {
			Entity entity(entityID, m_Scene.get());
			if (!entity)
				return;
			SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream ofs(filepath);
		ofs << out.c_str();
		ofs.close();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		BL_CORE_ASSERT(false, "not available");
	}

	bool SceneSerializer::DeSerialize(const std::string& filepath)
	{
		YAML::Node config = YAML::LoadFile(filepath);
		if (!config["Scene"])
			return false;

		std::string sceneName = config["Scene"].as<std::string>();
		BL_CORE_TRACE("Scene:{0} is deserialized",sceneName);

		auto entites = config["Entities"];
		if (entites) {
			for (auto entity : entites) {
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				Entity entt = m_Scene->CreateEntity();

				if (entity["TagComponent"]) {
					auto tagComponent = entity["TagComponent"];
					std::string name = tagComponent["Tag"].as<std::string>();

					entt.GetComponent<TagComponent>().Tag = name;
					BL_CORE_TRACE("Deserialized entity(uuid:{0},name:{1})", uuid, name);
				}

				if (entity["TransformComponent"]) {
					auto& tc = entt.GetComponent<TransformComponent>();
					auto transformComponent = entity["TransformComponent"];
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				if (entity["SpriteComponent"]) {
					auto& sc = entt.AddComponent<SpriteComponent>();
					auto spriteComponent = entity["SpriteComponent"];
					sc.Color = spriteComponent["Color"].as<glm::vec4>();
					if (spriteComponent["Texture"])
						sc.Texture = Texture2D::Create(spriteComponent["Texture"].as<std::string>());
					sc.TilingFactor = spriteComponent["TilingFactor"].as<float>();
				}

				if (entity["CameraComponent"]) {
					auto& cc = entt.AddComponent<CameraComponent>();
					auto cameraComponent = entity["CameraComponent"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraComponent["ProjectionType"].as<int>());
					cc.Camera.SetPerspectiveVerticalFov(cameraComponent["PerspectiveVerticalFov"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraComponent["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraComponent["PerspectiveFar"].as<float>());
					cc.Camera.SetOrthographicSize(cameraComponent["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraComponent["OrthographicNearClip"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraComponent["OrthographicFarClip"].as<float>());
					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
			}
		}
		return true;
	}

	bool SceneSerializer::DeSerializeRuntime(const std::string& filepath)
	{
		BL_CORE_ASSERT(false, "not available");
		return false;
	}
}

