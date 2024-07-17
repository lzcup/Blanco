#include "BLpch.h"
#include "SceneSerializer.h"
#include "Components.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

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
	static YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
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

	static std::string ProjectionTypeToString(SceneCamera::ProjectionType projectionType)
	{
		switch (projectionType)
		{
		case SceneCamera::ProjectionType::Perspective:
			return "Perspective";
		case SceneCamera::ProjectionType::Orthographic:
			return "Orthographic";
		}
		BL_CORE_ASSERT(false, "Unkonw ProjectionType");
		return "";
	}

	static SceneCamera::ProjectionType GetProjectionTypeFromString(const std::string& projectionTypeString)
	{
		if (projectionTypeString == "Perspective")
			return SceneCamera::ProjectionType::Perspective;
		if (projectionTypeString == "Orthographic")
			return SceneCamera::ProjectionType::Orthographic;
		BL_CORE_ASSERT(false, "Unkonw ProjectionTypeString");
		return SceneCamera::ProjectionType::Orthographic;
	}

	static std::string Rigidbody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:
			return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic:
			return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic:
			return "Kinematic";
		}
		BL_CORE_ASSERT(false, "Unkonw BodyType");
		return "";
	}

	static Rigidbody2DComponent::BodyType GetRigidbody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")
			return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")
			return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic")
			return Rigidbody2DComponent::BodyType::Kinematic;
		BL_CORE_ASSERT(false, "Unkonw BodyTypeString");
		return Rigidbody2DComponent::BodyType::Static;
	}

	static void SerializeEntity(YAML::Emitter& out,Entity& entity) {
		BL_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity without UUID");

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

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
			out << YAML::Key << "ProjectionType" << YAML::Value << ProjectionTypeToString(cc.Camera.GetProjectionType());
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
		if (entity.HasComponent<Rigidbody2DComponent>()) {
			auto& rc = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;  //Rigidbody2DComponent
			out << YAML::Key << "Type" << YAML::Value << Rigidbody2DBodyTypeToString(rc.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rc.FixedRotation;
			out << YAML::EndMap;  //Rigidbody2DComponent
		}
		if (entity.HasComponent<BoxCollider2DComponent>()) {
			auto& bc = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;  //BoxCollider2DComponent
			out << YAML::Key << "Offset" << YAML::Value << bc.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc.Size;
			out << YAML::Key << "Density" << YAML::Value << bc.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc.RestitutionThreshold;
			out << YAML::EndMap;  //BoxCollider2DComponent
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
				Entity entt = m_Scene->CreateEntityWithUUID(uuid);

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
					cc.Camera.SetProjectionType(GetProjectionTypeFromString(cameraComponent["ProjectionType"].as<std::string>()));
					cc.Camera.SetPerspectiveVerticalFov(cameraComponent["PerspectiveVerticalFov"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraComponent["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraComponent["PerspectiveFar"].as<float>());
					cc.Camera.SetOrthographicSize(cameraComponent["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraComponent["OrthographicNearClip"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraComponent["OrthographicFarClip"].as<float>());
					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				if (entity["Rigidbody2DComponent"]) {
					auto& rc = entt.AddComponent<Rigidbody2DComponent>();
					auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
					rc.Type = GetRigidbody2DBodyTypeFromString(rigidbody2DComponent["Type"].as<std::string>());
					rc.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				if (entity["BoxCollider2DComponent"]) {
					auto& bc = entt.AddComponent<BoxCollider2DComponent>();
					auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
					bc.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc.Density = boxCollider2DComponent["Density"].as<float>();
					bc.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
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

