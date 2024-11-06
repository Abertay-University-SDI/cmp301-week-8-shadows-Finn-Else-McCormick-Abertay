#include "DirectionalBlurShader.h"

DirectionalBlurShader::DirectionalBlurShader(const DeviceInfo& info, HWND hwnd) : PostprocessingShader(info, hwnd, "post/directionalBlur_post") {
	CreateConstantBuffer<DirectionalBlurBuffer, PIXEL_SHADER>(2);
}

std::unique_ptr<ISceneDataShader::ISceneData> DirectionalBlurShader::DefaultSceneData() const {
	return std::make_unique<DirectionalBlurData>();
}

void DirectionalBlurShader::__Internal_UploadSceneData(const ISceneData&) {
	auto& data = SceneDataAs<DirectionalBlurData>();

	DirectionalBlurBuffer buf{};
	buf.angle = data.angle;
	buf.distance = data.distance;
	buf.samples = data.samples;

	UploadConstantBuffer(buf);
}