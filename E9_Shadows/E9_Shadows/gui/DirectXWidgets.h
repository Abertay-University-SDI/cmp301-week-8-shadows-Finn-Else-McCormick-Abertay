#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <imGUI/imgui.h>

using namespace DirectX;

namespace ImGui {
	bool DragFloat2(const char* label, XMFLOAT2&, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
	bool DragFloat3(const char* label, XMFLOAT3&, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
	bool DragFloat4(const char* label, XMFLOAT4&, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);

	bool InputFloat2(const char* label, XMFLOAT2&, const char* format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
	bool InputFloat3(const char* label, XMFLOAT3&, const char* format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
	bool InputFloat4(const char* label, XMFLOAT4&, const char* format = "%.3f", ImGuiInputTextFlags extra_flags = 0);

	bool SliderFloat2(const char* label, XMFLOAT2&, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
	bool SliderFloat3(const char* label, XMFLOAT3&, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
	bool SliderFloat4(const char* label, XMFLOAT4&, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);

	bool DragInt2(const char* label, XMINT2&, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
	bool DragInt3(const char* label, XMINT3&, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
	bool DragInt4(const char* label, XMINT4&, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");

	bool InputInt2(const char* label, XMINT2&, ImGuiInputTextFlags extra_flags = 0);
	bool InputInt3(const char* label, XMINT3&, ImGuiInputTextFlags extra_flags = 0);
	bool InputInt4(const char* label, XMINT4&, ImGuiInputTextFlags extra_flags = 0);

	bool SliderInt2(const char* label, XMINT2&, int v_min, int v_max, const char* format = "%d");
	bool SliderInt3(const char* label, XMINT3&, int v_min, int v_max, const char* format = "%d");
	bool SliderInt4(const char* label, XMINT4&, int v_min, int v_max, const char* format = "%d");
}