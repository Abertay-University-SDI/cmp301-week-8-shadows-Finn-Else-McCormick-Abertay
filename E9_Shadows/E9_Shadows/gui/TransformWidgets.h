#pragma once

#include <Transform.h>

typedef int ImGuiTransformFlags;
enum ImGuiTransformFlags_ {
	ImGuiTransformFlags_None		= 0,
	ImGuiTransformFlags_NoPosition	= 1 << 0,
	ImGuiTransformFlags_NoScale		= 1 << 1,
	ImGuiTransformFlags_NoRotation	= 1 << 2,
	ImGuiTransformFlags_NoRoll		= 1 << 3,
};

namespace ImGui {
	bool InputTransform(Transform&, ImGuiTransformFlags flags = 0, float speed = 0.01f);

	bool InputTransform(ITransformed&, ImGuiTransformFlags flags = 0, float speed = 0.01f);
}