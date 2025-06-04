#include "JsonFunction.h"

bool JsonFunction::Save(bool pretty) const
{
    // ファイルを保存する
    std::ofstream ofs(kDirectoryPath_ + filePath_, std::ios::out | std::ios::trunc);
    if (!ofs) { std::cerr << "JsonFunction::Save 失敗: " << filePath_ << '\n'; return false; }
    ofs << (pretty ? data_.dump(4) : data_.dump());
    return true;
}

bool JsonFunction::Load() 
{
    // ファイルを探す
    std::ifstream ifs(kDirectoryPath_ + filePath_);
    if (!ifs) { std::cerr << "JsonFunction::Load 失敗: " << filePath_ << '\n'; return false; }
    try { ifs >> data_; }
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
