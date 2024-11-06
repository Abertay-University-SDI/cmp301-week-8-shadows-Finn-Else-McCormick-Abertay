#include "WideStringUtils.h"

#include <locale>
#include <vector>
#include <windows.h>

std::string to_string(const std::wstring& wstr) {
	std::vector<char> buf(wstr.size());
	// UTF-8 Encoding
	//std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	//return std::string(buf.data(), buf.size());

	// ANSI Encoding
	int sz = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), 0, 0, 0, 0);
	std::string result = std::string(sz, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &result[0], sz, 0, 0);
	return result;
}

std::string to_string(const wchar_t* wcstr) {
	return to_string(std::wstring(wcstr));
}

std::wstring to_wstring(const std::string& str) {
	std::vector<wchar_t> buf(str.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(str.data(), str.data() + str.size(), buf.data());
	return std::wstring(buf.data(), buf.size());
}

std::wstring to_wstring(const char* cstr) {
	return to_wstring(std::string(cstr));
}