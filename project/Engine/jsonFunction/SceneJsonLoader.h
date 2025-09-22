#pragma once
#include <json.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "JsonFunction.h"
#include "WorldTransform.h"
#include "Collider.h"
#include "Vector3.h"

// 保存するコライダーのデータ
struct ColliderData {
    ColliderType type;
    Vector3 center{};
    Vector3 size{};
    float radius;
    Vector2 defPosition;
    float rotate{};
    bool active;
};

// 保存するシーンオブジェクト
struct SceneObject {
    std::string type;                       // 何のObejctなのか
    std::string name;                       // そのObejctの名前
    std::string fileName;                   // Meshのファイルの名前
    std::string groupName;                  // どのObjectGroupに属しているか
    std::string tag;                        // Groupの中の仕分けtag
    WorldTransform transform;               // ObjectのTransform
    ColliderData collider;                  // 判定用データ
    std::vector<SceneObject> children;      // ペアレントされているObject再帰構造
};

class SceneJsonLoader
{
public:

    void Init(const std::string filePath) { filePath_ = filePath + ".json"; }

    void Load(const std::string filePath);

    // nameを入れてデータを取得する
    std::map<std::string, SceneObject>& GetData() { return sceneObjectDatas_; }

private:

    void ParseObject(const Json& json, SceneObject& object);

private:

    // 保存したデータ
    Json datas_;
    // データから作ったマップ
    std::map<std::string, SceneObject> sceneObjectDatas_;

    // 保存先ファイルパス
    const std::string kDirectoryPath_ = "resources/gameData/";
    std::string filePath_ = "";

};

