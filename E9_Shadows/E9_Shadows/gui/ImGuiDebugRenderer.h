#pragma once

#include <scene/Camera.h>

#include <vector>
#include <queue>
#include <memory>
#include <utility>
#include <functional>

#include <imgui/imgui.h>

struct IDebugDrawable {
	virtual void Draw(ImDrawList* drawList, Camera* camera) = 0;
	static std::pair<ImVec2, float> Project(XMFLOAT3, Camera* camera);
};

class ImGuiDebugRenderer
{
public:
	ImGuiDebugRenderer(Camera* camera);

	void SetCamera(Camera* camera);

	void Render();
	void Clear();

	void PushDrawable(std::unique_ptr<IDebugDrawable>&&);

private:
	Camera* p_camera;
	std::queue<std::unique_ptr<IDebugDrawable>> m_drawQueue;
};