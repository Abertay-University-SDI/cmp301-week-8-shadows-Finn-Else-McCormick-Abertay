#pragma once

#include <system/IApp.h>
#include <system/Renderer.h>
#include <system/RenderTarget.h>
#include <system/Timer.h>
#include <TextureManager.h>

#include <gui/ImGuiDebugRenderer.h>

#include <scene/ModelData.h>
#include <scene/LightData.h>
#include <scene/CameraController.h>
#include <scene/OrthoCamera.h>

#include <shaders/scene/SceneShader.h>

#include <shaders/post/PostprocessingShader.h>
#include <shaders/post/DirectionalBlurShader.h>

#include <vector>
#include <memory>

const float SCREEN_DEPTH = 200.0f;
const float SCREEN_NEAR = 0.1f;

class AppMain : public IApp
{
public:
	AppMain() = default;
	void Init() override;
	void Shutdown() override;

	bool Frame() override;

	void OnResized() override;

protected:
	bool Render();
	void Gui();

	void AddLight();

	std::unique_ptr<Renderer> pm_renderer;
	std::unique_ptr<Timer> pm_timer;
	std::unique_ptr<TextureManager> pm_textureMgr;

	void RenderModel(const ModelData&, const Camera&, const std::vector<LightData>& = std::vector<LightData>{}, const XMFLOAT3& ambientLight = XMFLOAT3(0.f, 0.f, 0.f));
	void RenderPass(IRenderTarget& target, const Camera& camera, bool clear, const std::vector<ModelData>& models, const std::vector<LightData>& lights = std::vector<LightData>{}, const XMFLOAT3& ambientLight = XMFLOAT3(0.f, 0.f, 0.f), const XMFLOAT4& clearColour = XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	void PostprocessPass(PostprocessingShader& shader, IRenderTarget* activeTargetOverride = nullptr);

private:
	std::unique_ptr<CameraController> pm_playerCamera;
	std::unique_ptr<OrthoCamera> pm_screenspaceCamera;
	std::unique_ptr<ImGuiDebugRenderer> pm_debugRenderer;

	std::unique_ptr<SceneShader> pm_lightShader;
	std::unique_ptr<PostprocessingShader> pm_postDefaultShader;
	std::unique_ptr<DirectionalBlurShader> pm_directionalBlurShader;

	std::vector<ModelData> m_models;
	std::unique_ptr<ModelData> pm_screenQuad;

	std::vector<LightData> m_lights;

	std::unique_ptr<RenderTarget> pm_targetFirst, pm_targetSecond; bool m_targetFirstActive = true;
	RenderTarget& ActiveTarget(); RenderTarget& BackTarget();

	bool m_wireframe = false;
	bool m_disableTextures = false;
	bool m_showNormals = false;
	float normalOffset = 0.f;
	bool m_debugDraw = false;
};