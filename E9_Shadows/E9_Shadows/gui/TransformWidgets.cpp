#include "TransformWidgets.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "DirectXWidgets.h"

bool ImGui::InputTransform(Transform& trans, ImGuiTransformFlags flags, float speed) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool transformChanged = false;
	ImGui::BeginGroup();
	ImGui::PushID(&trans);
	if (!(flags & ImGuiTransformFlags_NoPosition)) { transformChanged |= DragFloat3("Position", trans.position, speed); }

	if (!(flags & ImGuiTransformFlags_NoScale)) { transformChanged |= DragFloat3("Scale", trans.scale, speed); }

	if (!(flags & ImGuiTransformFlags_NoRotation)) {
		transformChanged |= ImGui::SliderAngle("Pitch", &trans.rotation.x, 0.f, 360.f);
		transformChanged |= ImGui::SliderAngle("Yaw  ", &trans.rotation.y, 0.f, 360.f);
		if (!(flags & ImGuiTransformFlags_NoRoll)) {
			transformChanged |= ImGui::SliderAngle("Roll ", &trans.rotation.z, 0.f, 360.f);
		}
	}
	ImGui::PopID();
	ImGui::EndGroup();
	return transformChanged;
}


bool ImGui::InputTransform(ITransformed& transformed, ImGuiTransformFlags flags, float speed) {
	return InputTransform(transformed.Trans(), flags, speed);
}