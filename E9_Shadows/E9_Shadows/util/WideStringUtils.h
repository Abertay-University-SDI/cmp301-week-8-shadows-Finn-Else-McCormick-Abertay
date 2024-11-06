#pragma once

#include <string>

std::string to_string(const std::wstring&);
std::string to_string(const wchar_t*);
std::wstring to_wstring(const std::string&);
std::wstring to_wstring(const char*);