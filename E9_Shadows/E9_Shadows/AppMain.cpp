// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "AppMain.h"

#include "DirectXMath.h"
#include <gui/TransformWidgets.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include <gui/DebugDrawables.h>

#include <geometry/CubeMesh.h>
#include <geometry/PlaneMesh.h>
#include <geometry/SphereMesh.h>
#include <geometry/QuadMesh.h>

#include <algorithm>

void AppMain::Init()
{
	pm_renderer = std::make_unique<Renderer>(m_width, m_height, true, hwnd, false);
	if (!pm_renderer)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		exit(EXIT_FAILURE);
	}

	// Initialise ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(pm_renderer->Device(), pm_renderer->DeviceContext());

	io.Fonts->AddFontDefault();

	// Setup Timer, Camera, etc

	auto deviceInfo = pm_renderer->DeviceInfo();

	pm_timer = std::make_unique<Timer>();

	pm_playerCamera = std::make_unique<CameraController>(p_input, hwnd, m_width, m_height, SCREEN_NEAR, SCREEN_DEPTH);
	pm_playerCamera->SetPosition(XMFLOAT3(0.f, 0.f, -10.f));

	pm_screenspaceCamera = std::make_unique<OrthoCamera>(m_width, m_height, SCREEN_NEAR, SCREEN_DEPTH, true);
	pm_screenspaceCamera->SetPosition(XMFLOAT3(m_width / 2.f, m_height / 2.f, 100.f));
	pm_screenspaceCamera->SetYaw(XMConvertToRadians(180.f));

	pm_debugRenderer = std::make_unique<ImGuiDebugRenderer>(pm_playerCamera.get());

	// Load texture
	pm_textureMgr = std::make_unique<TextureManager>(deviceInfo);
	pm_textureMgr->Load("brick", "res/brick1.dds");
	pm_textureMgr->Load("placeholder", "res/DefaultDiffuse.png");

	// Create shaders
	pm_lightShader = std::make_unique<Shader>(deviceInfo, hwnd, "scene/light_vs", "scene/light_ps");

	pm_postDefaultShader = std::make_unique<PostprocessingShader>(deviceInfo, hwnd);

	pm_directionalBlurShader = std::make_unique<DirectionalBlurShader>(deviceInfo, hwnd);
	pm_directionalBlurShader->SceneDataAs<DirectionalBlurShader::DirectionalBlurData>();

	// Create models
	m_models.push_back(make_model<CubeMesh>(*pm_lightShader, pm_textureMgr->Get("brick"), nullptr, deviceInfo));
	auto& cube = m_models.back();

	m_models.push_back(make_model<SphereMesh>(*pm_lightShader, pm_textureMgr->Get("placeholder"), nullptr, deviceInfo));
	auto& sphere = m_models.back();
	sphere.Trans().position = XMFLOAT3(2.f, 0.f, 5.f);

	m_models.push_back(make_model<PlaneMesh>(*pm_lightShader, pm_textureMgr->Get("brick"), nullptr, deviceInfo, 500));
	auto& plane = m_models.back();
	plane.Trans().SetScale(XMFLOAT3(10.f, 1.f, 10.f));
	plane.Trans().position.y = -1.f;


	// Targets
	pm_targetFirst = std::make_unique<RenderTarget>(deviceInfo, m_width, m_height);
	pm_targetSecond = std::make_unique<RenderTarget>(deviceInfo, m_width, m_height);

	pm_screenQuad = std::make_unique<ModelData>(std::move(make_model<QuadMesh>(*pm_postDefaultShader, pm_targetFirst->Texture(), nullptr, deviceInfo)));
	pm_screenQuad->Trans().SetScale(XMFLOAT3(m_width, m_height, 0.f));
	auto& screenData = pm_screenQuad->ShaderDataAs<PostprocessingShader::ScreenData>();
	screenData.width = pm_screenQuad->Trans().scale.x;
	screenData.height = pm_screenQuad->Trans().scale.y;
	
	// Initialise lights
	AddLight();
}

void AppMain::Shutdown() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


bool AppMain::Frame()
{
	if (p_input->isKeyDown(VK_ESCAPE)) { return false; }

	pm_timer->Frame();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	pm_playerCamera->Update(pm_timer->DeltaTime());

	for (int i = 0; i < m_lights.size(); ++i) {
		auto& light = m_lights.at(i);
		XMFLOAT3 d = light.Diffuse();
		XMFLOAT4 lightColour = XMFLOAT4( d.x, d.y, d.z, 1.f );

		XMFLOAT3 lightDir = light.Trans().Rotated(XMFLOAT3(0.f, -1.f, 0.f));
		XMStoreFloat3(&lightDir, XMLoadFloat3(&lightDir) * light.Intensity());

		if (!light.IsDirectional()) {
			pm_debugRenderer->PushDrawable(std::make_unique<Drawable::Point>(light.Trans().position, 10.f, lightColour));
		}
		if (light.UmbraAngle() < XM_PI) {
			pm_debugRenderer->PushDrawable(std::make_unique<Drawable::Arrow>(light.Trans().position, lightDir, 5.f, lightColour));
		}
	}	

	Gui();

	return Render();
}

void AppMain::OnResized() {
	ImGui_ImplDX11_InvalidateDeviceObjects();

	pm_renderer->rebuild(m_width, m_height);

	ImGui_ImplDX11_CreateDeviceObjects();

	pm_playerCamera->SetViewport(m_width, m_height);

	pm_screenspaceCamera->SetViewport(m_width, m_height);
	pm_screenspaceCamera->SetPosition(XMFLOAT3(m_width / 2.f, m_height / 2.f, 100.f));

	pm_targetFirst = std::make_unique<RenderTarget>(pm_renderer->DeviceInfo(), m_width, m_height);
	pm_targetSecond = std::make_unique<RenderTarget>(pm_renderer->DeviceInfo(), m_width, m_height);

	pm_screenQuad->SetTexture(pm_targetFirst->Texture());
	pm_screenQuad->Trans().SetScale(XMFLOAT3(m_width, m_height, 0.f));
	auto& screenData = pm_screenQuad->ShaderDataAs<PostprocessingShader::ScreenData>();
	screenData.width = pm_screenQuad->Trans().scale.x;
	screenData.height = pm_screenQuad->Trans().scale.y;

	Frame();
}

void AppMain::RenderModel(const ModelData& model, const Camera& camera, const std::vector<LightData>& lights, const XMFLOAT3& ambientLight) {
	if (!model.ShouldRender()) { return; }

	auto& shader = model.Shader();

	shader.UploadMatrixData(model.Trans().AsMatrix(), camera.ViewMatrix(), camera.ProjectionMatrix());
	shader.UploadTimeData(pm_timer->ElapsedTime(), (float)m_showNormals, (float)m_disableTextures, normalOffset);
	shader.UploadLightData(lights, ambientLight, camera);
	shader.UploadTextureData(model.Texture(), model.NormalMap());
	shader.UploadExtraModelData(model.ShaderData());
	shader.UploadExtraSceneData();

	shader.Render(model.Mesh());
}

void AppMain::RenderPass(IRenderTarget& target, const Camera& camera, bool clear, const std::vector<ModelData>& models, const std::vector<LightData>& lights, const XMFLOAT3& ambientLight, const XMFLOAT4& clearColour) {
	target.Bind();
	if (clear) { target.Clear(clearColour); }

	for (auto& model : models) {
		RenderModel(model, camera, lights, ambientLight);
	}
}

RenderTarget& AppMain::ActiveTarget() { return (m_targetFirstActive ? *pm_targetFirst : *pm_targetSecond); }
RenderTarget& AppMain::BackTarget() { return (m_targetFirstActive ? *pm_targetSecond : *pm_targetFirst); }

void AppMain::PostprocessPass(Shader& shader, IRenderTarget* activeTarget) {
	if (!activeTarget) { activeTarget = &ActiveTarget(); }

	// Wireframe mode is applied to scene rendering but not postprocessing
	activeTarget->SetWireframe(false);
	BackTarget().SetWireframe(m_wireframe);

	pm_screenQuad->SetTexture(BackTarget().Texture());
	pm_screenQuad->SetShader(&shader);
	activeTarget->Bind(); activeTarget->Clear();
	RenderModel(*pm_screenQuad, *pm_screenspaceCamera);

	// Flip targets
	m_targetFirstActive = !m_targetFirstActive;
}

bool AppMain::Render()
{
	// Main Pass
	RenderPass(BackTarget(), *pm_playerCamera, true, m_models, m_lights, XMFLOAT3(0.2f, 0.2f, 0.2f), XMFLOAT4(0.39f, 0.58f, 0.92f, 1.f));

	// Blur passes
	PostprocessPass(*pm_directionalBlurShader);

	// Render to Backbuffer
	PostprocessPass(*pm_postDefaultShader, &pm_renderer->BackBuffer());

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	pm_renderer->SwapBuffers();

	return true;
}

void AppMain::Gui()
{
	// Force turn off unnecessary shader stages.
	pm_renderer->DeviceContext()->GSSetShader(NULL, NULL, 0);
	pm_renderer->DeviceContext()->HSSetShader(NULL, NULL, 0);
	pm_renderer->DeviceContext()->DSSetShader(NULL, NULL, 0);

	// Debug rendering
	if (m_debugDraw) { pm_debugRenderer->Render(); } else { pm_debugRenderer->Clear(); }

	// FPS readout
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGui::SetNextWindowBgAlpha(0.f);
	ImGui::Begin("_readout", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text("FPS: %.2f", pm_timer->FPS());

	auto& io = ImGui::GetIO();
	if (io.KeyCtrl) {
		ImGui::SameLine(100.f);
		ImGui::Text("Mouse Pos: %i, %i", (int)io.MousePos.x, (int)io.MousePos.y);
	}
	/*
	ImGui::SameLine();
	XMFLOAT3 camPos = pm_playerCamera->Position();
	ImGui::Text("x: %.2f, y: %.2f, z: %.2f", camPos.x, camPos.y, camPos.z);
	*/
	ImGui::PopStyleVar();
	ImGui::End();

	auto lightSection = [](LightData& light) {
		ImGui::PushID(&light);
		if (ImGui::CollapsingHeader(("Light " + std::to_string((int)&light)).c_str())) {

			Transform trans = light.Trans();
			if (ImGui::InputTransform(trans, ImGuiTransformFlags_NoScale | ImGuiTransformFlags_NoRoll)) {
				light.SetTrans(trans);
			}

			bool directional = light.IsDirectional();
			if (ImGui::Checkbox("Directional", &directional)) {
				light.SetIsDirectional(directional);
			}

			float intensity = light.Intensity();
			if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 1000.f)) { light.SetIntensity(intensity); }

			float specPower = light.SpecularPower();
			if (ImGui::DragFloat("Specular Power", &specPower, 0.01f, 0.f, 1000.f)) { light.SetSpecularPower(specPower); }

			XMFLOAT3 distanceFalloff = light.DistanceFalloff();
			float distFalloffTemp[3] = { distanceFalloff.x, distanceFalloff.y, distanceFalloff.z };

			ImGui::Text("Distance Falloff");
			ImGui::PushID("Distance");
			bool updateDist = false;
			updateDist |= ImGui::DragFloat("Constant", &distFalloffTemp[0], 0.01f, 0.f, 1000.f);
			updateDist |= ImGui::DragFloat("Linear", &distFalloffTemp[1], 0.01f, 0.f, 1000.f);
			updateDist |= ImGui::DragFloat("Quadratic", &distFalloffTemp[2], 0.01f, 0.f, 1000.f);
			if (updateDist) {
				light.SetDistanceFalloff(XMFLOAT3(distFalloffTemp[0], distFalloffTemp[1], distFalloffTemp[2]));
			}
			ImGui::PopID();

			float umbraAngle = light.UmbraAngle();
			float penumbraAngle = light.PenumbraAngle();

			ImGui::Text("Spotlight Falloff");
			if (ImGui::SliderFloat("Umbra Angle", &umbraAngle, 0.f, XM_PI, "%.3f rad")) {
				light.SetUmbraAngle(umbraAngle);
			}
			if (ImGui::SliderFloat("Penumbra Angle", &penumbraAngle, 0.f, XM_PI, "%.3f rad")) {
				light.SetPenumbraAngle(penumbraAngle);
			}
			if (ImGui::CollapsingHeader("Colour")) {
				XMFLOAT3 diffuse = light.Diffuse(); float diffuseTemp[3] = { diffuse.x, diffuse.y, diffuse.z };
				if (ImGui::ColorPicker3("Diffuse", diffuseTemp)) {
					light.SetDiffuse(XMFLOAT3(diffuseTemp[0], diffuseTemp[1], diffuseTemp[2]));
				}

				XMFLOAT3 specular = light.Specular(); float specularTemp[3] = { specular.x, specular.y, specular.z };
				if (ImGui::ColorPicker3("Specular", specularTemp)) {
					light.SetSpecular(XMFLOAT3(specularTemp[0], specularTemp[1], specularTemp[2]));
				}
			}
		}
		ImGui::PopID();
	};

	auto modelSection = [](ModelData& model, float speed = 0.01f) {
		ImGui::PushID(&model);
		if (ImGui::CollapsingHeader(("Model " + std::to_string((int)&model)).c_str())) {
			ImGui::InputTransform(model.Trans(), ImGuiTransformFlags_None, speed);
			bool hidden = !model.ShouldRender();
			if (ImGui::Checkbox("Hidden", &hidden)) {
				model.SetShouldRender(!hidden);
			}
		}
		ImGui::PopID();
	};


	ImGui::SetNextWindowPos(ImVec2(320.f, 25.f), ImGuiCond_Once);
	if (ImGui::Begin("Debug", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Checkbox("Wireframe", &m_wireframe);
		ImGui::Checkbox("Disable Textures", &m_disableTextures);
		ImGui::Checkbox("Display Normals", &m_showNormals);
		ImGui::Checkbox("Debug Draw", &m_debugDraw);
		//ImGui::DragFloat("Normal Offset", &normalOffset, 0.01f, -1.f, 1.f);
	}
	ImGui::End();

	

	ImGui::SetNextWindowPos(ImVec2(475.f, 25.f), ImGuiCond_Once);
	if (ImGui::Begin("Models", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		for (auto& model : m_models) {
			modelSection(model);
		}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(700.f, 25.f), ImGuiCond_Once);
	if (ImGui::Begin("Lights", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		for (auto& light : m_lights) {
			lightSection(light);
		}
		if (m_lights.size() < Shader::NUM_LIGHTS) {
			if (ImGui::Button("Add Light")) {
				AddLight();
			}
		}
	}
	ImGui::End();

	auto cameraSection = [](Camera& camera, const char* name) {
		auto viewport = camera.Viewport();
		ImGui::PushID(&camera);
		if (ImGui::CollapsingHeader(name)) {
			int viewportVals[2] = { viewport.first, viewport.second };
			ImGui::InputInt2("Viewport", viewportVals, ImGuiInputTextFlags_ReadOnly);
			float nearFarVals[2] = { camera.NearPlane(), camera.FarPlane() };
			if (ImGui::InputFloat2("Clipping Planes", nearFarVals)) {
				camera.SetNearPlane(nearFarVals[0]);
				camera.SetFarPlane(nearFarVals[1]);
			}
			float fieldOfView = camera.FieldOfView();
			if (ImGui::SliderAngle("FOV", &fieldOfView, 1.f, 180.f)) {
				camera.SetFieldOfView(fieldOfView);
			}
			if (ImGui::InputTransform(camera)) {
				camera.RecalculateViewMatrix();
			}
		}
		ImGui::PopID();
	};

	ImGui::SetNextWindowPos(ImVec2(700.f, 100.f), ImGuiCond_Once);
	if (ImGui::Begin("Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		cameraSection(*pm_playerCamera, "Player");
		cameraSection(*pm_screenspaceCamera, "Screenspace");
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(900.f, 100.f), ImGuiCond_Once);
	if (ImGui::Begin("Blur", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		auto& data = pm_directionalBlurShader->SceneDataAs<DirectionalBlurShader::DirectionalBlurData>();

		static bool autoSamples = true;
		static float sampleMult = 0.5f;

		auto calcSamples = [&]() {
			if (!autoSamples) { return; }
			data.samples = (int)(std::max)(data.distance * sampleMult, 1.f);
		};

		if (ImGui::SliderAngle("Angle", &data.angle, 0.f, 360.f)) { calcSamples(); }
		if (ImGui::DragFloat("Distance", &data.distance, 1.f, 0.f, 1000.f)) { calcSamples(); }
		ImGui::DragInt("Samples", &data.samples, 0.5f, 1, 100);
		ImGui::SameLine();
		if (ImGui::Checkbox("Auto", &autoSamples)) { calcSamples(); }
		if (autoSamples) {
			if (ImGui::DragFloat("Sample Multiplier", &sampleMult, 0.01f, 0.f, 10.f)) { calcSamples(); }
		}

	}
	ImGui::End();
}

void AppMain::AddLight() {
	m_lights.push_back(LightData());
	auto& light = m_lights.back();
	light.SetDiffuse(XMFLOAT3(1.0f, 1.0f, 1.0f));
	light.SetSpecular(XMFLOAT3(1.0f, 1.0f, 1.0f));
	light.SetDistanceFalloff(XMFLOAT3(0.f, 0.2f, 0.f));

	Transform lightTrans = light.Trans();
	lightTrans.position.y = 4.f;
	light.SetTrans(lightTrans);
}