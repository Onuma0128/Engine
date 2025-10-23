#pragma once
#include <json.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

using Json = nlohmann::ordered_json;

// 自作構造体のシリアライズ設定
namespace nlohmann {

    // Vector2
    template <>
    struct adl_serializer<Vector2> {
        static void to_json(Json& j, const Vector2& v) {
            j = Json{ {"x", v.x}, {"y", v.y} };
        }
        static void from_json(const Json& j, Vector2& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
        }
    };
    // Vector3
    template <>
    struct adl_serializer<Vector3> {
        static void to_json(Json& j, const Vector3& v) {
            j = Json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
        }
        static void from_json(const Json& j, Vector3& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
        }
    };
    // Vector4
    template <>
    struct adl_serializer<Vector4> {
        static void to_json(Json& j, const Vector4& v) {
            j = Json{ {"x", v.x}, {"y", v.y}, {"z", v.z},{"w", v.w} };
        }
        static void from_json(const Json& j, Vector4& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
            j.at("w").get_to(v.w);
        }
    };
    // Quaternion
    template <>
    struct adl_serializer<Quaternion> {
        static void to_json(Json& j, const Quaternion& v) {
            j = Json{ {"x", v.x}, {"y", v.y}, {"z", v.z},{"w", v.w} };
        }
        static void from_json(const Json& j, Quaternion& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
            j.at("w").get_to(v.w);
        }
    };
}

/// <summary>
/// JSON操作を行うクラス
/// </summary>
class JsonFunction
{
public:

    // コンストラクタでファイルパスを取得する
    void Init(const std::string filePath) { filePath_ = filePath + ".json"; }

    // 値をセット
    void Set(const std::string& key, int value) { datas_[key] = value; }
    void Set(const std::string& key, float value) { datas_[key] = value; }
    void Set(const std::string& key, bool value) { datas_[key] = value; }
    void Set(const std::string& key, const char* value) { datas_[key] = static_cast<std::string>(value); }
    void Set(const std::string& key, const std::string value) { datas_[key] = value; }
    // 自作構造体(Vector2,3,4,Quaternion)
    void Set(const std::string& key, const Vector2& value) { datas_[key] = value; }
    void Set(const std::string& key, const Vector3& value) { datas_[key] = value; }
    void Set(const std::string& key, const Vector4& value) { datas_[key] = value; }
    void Set(const std::string& key, const Quaternion& value) { datas_[key] = value; }

    // 値を取得
    std::string GetString(const std::string& key, const std::string& defaultValue = "") const { 
        return GetValue<std::string>(key, defaultValue); 
    }
    template <class Item>
    Item Get(const std::string& key, const Item& defaultValue = {})const { 
        return GetValue<Item>(key, defaultValue);
    }

    // 今入っているデータのImGuiを描画する
    void DrawImGui(float dragSpeed = 0.01f);

    // JSON をファイルへ保存
    bool Save(bool pretty = true) const;

    // ファイルからロード
    bool Load();

    // データを消去
    void Clear() { datas_.clear(); }

    // データをファイル上から削除する
    bool Deleta() const;

private:
    template <class T>
    T GetValue(const std::string& key, const T& defaultValue) const {
        if (!datas_.contains(key)) return defaultValue;
        try { return datas_.at(key).get<T>(); }
        catch (...) { return defaultValue; }
    }

    // 保存するデータ
    Json datas_;
    // 保存先ファイルパス
    const std::string kDirectoryPath_ = "resources/gameData/";
    std::string filePath_ = "";

};

