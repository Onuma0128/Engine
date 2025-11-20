#pragma once
#include <string>

/// <summary>
/// ログ出力クラス
/// </summary>
class Logger {
public:

	/// <summary>
	/// ログを出力する
	/// </summary>
	/// <param name="message"></param>
	static void Log(const std::string& message);
};

