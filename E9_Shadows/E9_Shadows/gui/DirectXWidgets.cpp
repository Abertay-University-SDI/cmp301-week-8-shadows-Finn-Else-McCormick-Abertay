#include "DirectXWidgets.h"

/*-------*/
/* FLOAT */
/*-------*/

// Drag

bool ImGui::DragFloat2(const char* label, XMFLOAT2& vec, float v_speed, float v_min, float v_max, const char* format, float power) {
	float temp[2] = { vec.x, vec.y };
	bool changed = ImGui::DragFloat2(label, temp, v_speed, v_min, v_max, format, power);
	if (changed) { vec = XMFLOAT2(temp); }
	return changed;
}

bool ImGui::DragFloat3(const char* label, XMFLOAT3& vec, float v_speed, float v_min, float v_max, const char* format, float power) {
	float temp[3] = { vec.x, vec.y, vec.z };
	bool changed = ImGui::DragFloat3(label, temp, v_speed, v_min, v_max, format, power);
	if (changed) { vec = XMFLOAT3(temp); }
	return changed;
}

bool ImGui::DragFloat4(const char* label, XMFLOAT4& vec, float v_speed, float v_min, float v_max, const char* format, float power) {
	float temp[4] = { vec.x, vec.y, vec.z, vec.w };
	bool changed = ImGui::DragFloat4(label, temp, v_speed, v_min, v_max, format, power);
	if (changed) { vec = XMFLOAT4(temp); }
	return changed;
}

// Input

bool ImGui::InputFloat2(const char* label, XMFLOAT2& vec, const char* format, ImGuiInputTextFlags extra_flags)
{
	float temp[2] = { vec.x, vec.y };
	bool changed = ImGui::InputFloat2(label, temp, format, extra_flags);
	if (changed) { vec = XMFLOAT2(temp); }
	return changed;
}

bool ImGui::InputFloat3(const char* label, XMFLOAT3& vec, const char* format, ImGuiInputTextFlags extra_flags)
{
	float temp[3] = { vec.x, vec.y, vec.z };
	bool changed = ImGui::InputFloat3(label, temp, format, extra_flags);
	if (changed) { vec = XMFLOAT3(temp); }
	return changed;
}

bool ImGui::InputFloat4(const char* label, XMFLOAT4& vec, const char* format, ImGuiInputTextFlags extra_flags)
{
	float temp[4] = { vec.x, vec.y, vec.z, vec.w };
	bool changed = ImGui::InputFloat4(label, temp, format, extra_flags);
	if (changed) { vec = XMFLOAT4(temp); }
	return changed;
}

// Slider

bool ImGui::SliderFloat2(const char* label, XMFLOAT2& vec, float v_min, float v_max, const char* format, float power)
{
	float temp[2] = { vec.x, vec.y };
	bool changed = ImGui::SliderFloat2(label, temp, v_min, v_max, format, power);
	if (changed) { vec = XMFLOAT2(temp); }
	return changed;
}

bool ImGui::SliderFloat3(const char* label, XMFLOAT3& vec, float v_min, float v_max, const char* format, float power)
{
	float temp[3] = { vec.x, vec.y, vec.z };
	bool changed = ImGui::SliderFloat3(label, temp, v_min, v_max, format, power);
	if (changed) { vec = XMFLOAT3(temp); }
	return changed;
}

bool ImGui::SliderFloat4(const char* label, XMFLOAT4& vec, float v_min, float v_max, const char* format, float power)
{
	float temp[4] = { vec.x, vec.y, vec.z, vec.w };
	bool changed = ImGui::SliderFloat4(label, temp, v_min, v_max, format, power);
	if (changed) { vec = XMFLOAT4(temp); }
	return changed;
}

/*-----*/
/* INT */
/*-----*/

// Drag

bool ImGui::DragInt2(const char* label, XMINT2& vec, float v_speed, int v_min, int v_max, const char* format)
{
	int temp[2] = { vec.x, vec.y };
	bool changed = ImGui::DragInt2(label, temp, v_speed, v_min, v_max, format);
	if (changed) { vec = XMINT2(temp); }
	return changed;
}

bool ImGui::DragInt3(const char* label, XMINT3& vec, float v_speed, int v_min, int v_max, const char* format)
{
	int temp[3] = { vec.x, vec.y, vec.z };
	bool changed = ImGui::DragInt3(label, temp, v_speed, v_min, v_max, format);
	if (changed) { vec = XMINT3(temp); }
	return changed;
}

bool ImGui::DragInt4(const char* label, XMINT4& vec, float v_speed, int v_min, int v_max, const char* format)
{
	int temp[4] = { vec.x, vec.y, vec.z, vec.w };
	bool changed = ImGui::DragInt4(label, temp, v_speed, v_min, v_max, format);
	if (changed) { vec = XMINT4(temp); }
	return changed;
}

// Input

bool ImGui::InputInt2(const char* label, XMINT2& vec, ImGuiInputTextFlags extra_flags)
{
	int temp[2] = { vec.x, vec.y };
	bool changed = ImGui::InputInt2(label, temp, extra_flags);
	if (changed) { vec = XMINT2(temp); }
	return changed;
}

bool ImGui::InputInt3(const char* label, XMINT3& vec, ImGuiInputTextFlags extra_flags)
{
	int temp[3] = { vec.x, vec.y, vec.z };
	bool changed = ImGui::InputInt3(label, temp, extra_flags);
	if (changed) { vec = XMINT3(temp); }
	return changed;
}

bool ImGui::InputInt4(const char* label, XMINT4& vec, ImGuiInputTextFlags extra_flags)
{
	int temp[4] = { vec.x, vec.y, vec.z, vec.w };
	bool changed = ImGui::InputInt4(label, temp, extra_flags);
	if (changed) { vec = XMINT4(temp); }
	return changed;
}

// Slider

bool ImGui::SliderInt2(const char* label, XMINT2& vec, int v_min, int v_max, const char* format)
{
	int temp[2] = { vec.x, vec.y };
	bool changed = ImGui::SliderInt2(label, temp, v_min, v_max, format);
	if (changed) { vec = XMINT2(temp); }
	return changed;
}

bool ImGui::SliderInt3(const char* label, XMINT3& vec, int v_min, int v_max, const char* format)
{
	int temp[3] = { vec.x, vec.y, vec.z };
	bool changed = ImGui::SliderInt3(label, temp, v_min, v_max, format);
	if (changed) { vec = XMINT3(temp); }
	return changed;
}

bool ImGui::SliderInt4(const char* label, XMINT4& vec, int v_min, int v_max, const char* format)
{
	int temp[4] = { vec.x, vec.y, vec.z, vec.w };
	bool changed = ImGui::SliderInt4(label, temp, v_min, v_max, format);
	if (changed) { vec = XMINT4(temp); }
	return changed;
}