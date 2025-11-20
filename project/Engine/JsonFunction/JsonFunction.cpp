#include "JsonFunction.h"

#include "Imgui.h"

void JsonFunction::DrawImGui(float dragSpeed)
{
    for (auto& [key, val] : datas_.items())
    {
        // ---------- 数値 ----------
        if (val.is_number_integer())
        {
            int v = val.get<int>();
            if (ImGui::DragInt(key.c_str(), &v, 1.0f))
                val = v;
        } else if (val.is_number_float())
        {
            float v = val.get<float>();
            if (ImGui::DragFloat(key.c_str(), &v, dragSpeed))
                val = v;
        }
        // ---------- bool ----------
        else if (val.is_boolean())
        {
            bool v = val.get<bool>();
            if (ImGui::Checkbox(key.c_str(), &v))
                val = v;
        }
        // ---------- Vector / Quaternion ----------
        else if (val.is_object())
        {
            // 必須キーを確認
            const bool hasX = val.contains("x");
            const bool hasY = val.contains("y");
            const bool hasZ = val.contains("z");
            const bool hasW = val.contains("w");

            if (hasX && hasY && !hasZ && !hasW)            // Vector2
            {
                float v[2] = { val["x"], val["y"] };
                if (ImGui::DragFloat2(key.c_str(), v, dragSpeed))
                {
                    val["x"] = v[0]; val["y"] = v[1];
                }
            } else if (hasX && hasY && hasZ && !hasW)        // Vector3
            {
                float v[3] = { val["x"], val["y"], val["z"] };
                if (ImGui::DragFloat3(key.c_str(), v, dragSpeed))
                {
                    val["x"] = v[0]; val["y"] = v[1]; val["z"] = v[2];
                }
            } else if (hasX && hasY && hasZ && hasW)         // Vector4 or Quaternion
            {
                float v[4] = { val["x"], val["y"], val["z"], val["w"] };
                if (ImGui::DragFloat4(key.c_str(), v, dragSpeed))
                {
                    val["x"] = v[0]; val["y"] = v[1];
                    val["z"] = v[2]; val["w"] = v[3];
                }
            }
        }
    }
}


bool JsonFunction::Save(bool pretty) const
{
    // ファイルを保存する
    std::ofstream ofs(kDirectoryPath_ + filePath_, std::ios::out | std::ios::trunc);
    if (!ofs) { std::cerr << "JsonFunction::Save 失敗: " << filePath_ << '\n'; return false; }
    ofs << (pretty ? datas_.dump(4) : datas_.dump());
    return true;
}

bool JsonFunction::Load() 
{
    // ファイルを探す
    std::ifstream ifs(kDirectoryPath_ + filePath_);
    if (!ifs) { std::cerr << "JsonFunction::Load 失敗: " << filePath_ << '\n'; return false; }
    try { ifs >> datas_; }
    catch (const std::exception& e) {
        std::cerr << "JsonFunction::Load 解析エラー: " << e.what() << '\n'; return false;
    }
    return true;
}

bool JsonFunction::Deleta() const
{
    std::error_code ec;
    // 初期化時に決めたファイルパスのjsonを削除する
    std::filesystem::path filePath = kDirectoryPath_ + filePath_;
    bool removed = std::filesystem::remove(filePath, ec);
    if (!removed || ec) {
        std::cerr << "JsonFunction::DeleteFile 失敗: "
            << filePath_ << " (" << ec.message() << ")\n";
    }
    return removed && !ec;
}
