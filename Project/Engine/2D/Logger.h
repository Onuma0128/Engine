#pragma once
#include <string>

/// <summary>
/// ログ出力クラス
/// </summary>
namespace NumaEngine {
	class Logger {
	public:

		/// <summary>
		/// ログを出力する
		/// </summary>
		/// <param name="message"></param>
		static void Log(const std::string& message);
	};
} // namespace NumaEngine

// NOTE: Do not provide global alias. Use fully-qualified name `NumaEngine::Logger`.

