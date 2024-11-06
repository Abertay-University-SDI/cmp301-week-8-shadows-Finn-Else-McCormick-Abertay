#pragma once

#include "ImGuiDebugRenderer.h"

namespace Drawable {

	struct Point : IDebugDrawable {
		XMFLOAT3 pos; float size; XMFLOAT4 colour;
		Point(XMFLOAT3, float size = 1.f, XMFLOAT4 colour = XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		virtual void Draw(ImDrawList* drawList, Camera* camera) override;
	};

	struct Arrow : IDebugDrawable {
		XMFLOAT3 start, dir; float thickness; XMFLOAT4 colour;
		Arrow(XMFLOAT3 start, XMFLOAT3 dir, float thickness = 1.f, XMFLOAT4 colour = XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		virtual void Draw(ImDrawList* drawList, Camera* camera) override;
	};
}