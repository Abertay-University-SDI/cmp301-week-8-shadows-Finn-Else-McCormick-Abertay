#include "ImGuiDebugRenderer.h"

ImGuiDebugRenderer::ImGuiDebugRenderer(Camera* camera) {
	SetCamera(camera);
}

void ImGuiDebugRenderer::SetCamera(Camera* camera) {
	p_camera = camera;
}

void ImGuiDebugRenderer::PushDrawable(std::unique_ptr<IDebugDrawable>&& drawable) {
	m_drawQueue.push(std::move(drawable));
}

void ImGuiDebugRenderer::Render() {	
	auto viewport = p_camera->Viewport();
	int width = viewport.first, height = viewport.second;

	// Invisible, intangible window covering whole frame
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowBgAlpha(0.f);
	ImGui::Begin("_debugDraw", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings);
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		while (!m_drawQueue.empty()) {
			auto& drawable = m_drawQueue.front();

			drawable->Draw(drawList, p_camera);

			m_drawQueue.pop();
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void ImGuiDebugRenderer::Clear() {
	while (!m_drawQueue.empty()) {
		m_drawQueue.pop();
	}
}



std::pair<ImVec2, float> IDebugDrawable::Project(XMFLOAT3 pos, Camera* camera) {
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX viewMatrix = camera->ViewMatrix();
	XMMATRIX projMatrix = camera->ProjectionMatrix();

	auto viewport = camera->Viewport();
	int width = viewport.first, height = viewport.second;
	float near = camera->NearPlane(), far = camera->FarPlane();

	XMVECTOR vec = XMLoadFloat3(&pos);
	vec = XMVector3Project(vec, 0.f, 0.f, width, height, near, far, projMatrix, viewMatrix, worldMatrix);
	XMStoreFloat3(&pos, vec);

	return std::make_pair(ImVec2(pos.x, pos.y), pos.z / far);
}