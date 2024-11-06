#include "WideStringUtils.h"

#include <codecvt>
#include <locale>

std::string to_string(const std::wstring& wstr) {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
}

std::string to_string(const wchar_t* wcstr) {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wcstr);
}

std::wstring to_wstring(const std::string& str) {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

std::wstring to_wstring(const char* cstr) {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(cstr);
}