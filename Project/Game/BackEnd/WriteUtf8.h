#pragma once
#include <Windows.h>
#include <string>

// レスポンスを格納するためのコールバック関数
size_t WriteCallback(void* c, size_t s, size_t n, std::string * o) {
	o->append((char*)c, s * n);
	return s * n;
}

std::wstring ConvertString(const std::string& s) {
	int len = MultiByteToWideChar(
		CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
	std::wstring ws(len, 0);
	MultiByteToWideChar(
		CP_UTF8, 0, s.data(), (int)s.size(), &ws[0], len);

	return ws;
}

void WriteUtf8(const std::string& s) {
	std::wstring ws = ConvertString(s);
	DWORD _;
	WriteConsoleW(GetStdHandle(
		STD_OUTPUT_HANDLE), ws.c_str(),
		(DWORD)ws.size(), &_, nullptr);
}