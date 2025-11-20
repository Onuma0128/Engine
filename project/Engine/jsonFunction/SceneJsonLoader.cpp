#include "SceneJsonLoader.h"

#include <assert.h>
#include <numbers>

#include "imgui.h"

void SceneJsonLoader::Load(const std::string filePath)
{
	// ファイルのフルパスを取得
	filePath_ = filePath + ".json";
	const std::string fullpath = kDirectoryPath_ + filePath_;
	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	if (file.fail()) {
		assert(0);
	}
	// ファイルを解凍
	file >> datas_;

	// 正しいレベルデータファイルかチェック
	assert(datas_.is_object());
	assert(datas_.contains("name"));
	assert(datas_["name"].is_string());

	// "name"を文字列として取得
	std::string name = datas_["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// "objects"の全オブジェクトを走査
	for (Json& object : datas_["objects"]) {
		assert(object.contains("type"));
		// 読み込んだデータを移す
		SceneObject obj;
		ParseObject(object, obj);
		// データをマップで保存
		sceneObjectDatas_[obj.name] = obj;
	}
}

void SceneJsonLoader::ParseObject(const Json& json, SceneObject& object)
{
	object.type = json["type"].get<std::string>();
	object.name = json["name"].get<std::string>();

	if (json.contains("file_name")) {
		object.fileName = json["file_name"].get<std::string>();
	}
	if (json.contains("group_name")) {
		object.groupName = json["group_name"].get<std::string>();
	}
	if (json.contains("tag_name")) {
		object.tag = json["tag_name"].get<std::string>();
	}

	Vector2 defPos{};
	float defRotate = 0.0f;
	if (json.contains("transform")) {
		auto& transform = json["transform"];

		Vector3 translation = {
			static_cast<float>(transform["translation"][0]),
			static_cast<float>(transform["translation"][2]),
			static_cast<float>(transform["translation"][1]),
		};
		Vector3 rotate = {
			-static_cast<float>(transform["rotation"][0]),
			-static_cast<float>(transform["rotation"][2]),
			-static_cast<float>(transform["rotation"][1]),
		};
		Vector3 scaling = {
			static_cast<float>(transform["scaling"][0]),
			static_cast<float>(transform["scaling"][2]),
			static_cast<float>(transform["scaling"][1]),
		};

		object.transform.translation_ = translation;
		object.transform.rotation_ = Vector3::FromEuler(rotate);
		object.transform.scale_ = scaling;
		defPos = { translation.x,translation.z };
		defRotate = rotate.y;
	}

	if (json.contains("collider")) {
		ColliderData col;
		const auto& c = json["collider"];
		std::string type = c["type"].get<std::string>();
		if (type == "Box") { 
			col.type = ColliderType::kOBB;
			Vector3 size = {
			static_cast<float>(c["size"][0]),
			static_cast<float>(c["size"][2]),
			static_cast<float>(c["size"][1]),
			};
			col.size = size;

		} else if (type == "Sphere") { 
			col.type = ColliderType::kSphere; 
			float radius = static_cast<float>(c["radius"]);
			col.radius = radius;
		}

		Vector3 center = {
			static_cast<float>(c["center"][0]),
			static_cast<float>(c["center"][2]),
			static_cast<float>(c["center"][1]),
		};

		col.center = center;
		col.active = true;
		col.defPosition = defPos;
		col.rotate = defRotate * (std::numbers::pi_v<float> / 180.0f);
		object.collider = col;
	} else {
		object.collider.active = false;
	}

	if (json.contains("children")) {
		for (const auto& child : json["children"]) {
			SceneObject cobj;
			ParseObject(child, cobj);
			object.children.push_back(std::move(cobj));
		}
	}
}
