#include "DirectionalBlurShader.h"

DirectionalBlurShader::DirectionalBlurShader(const DeviceInfo& info, HWND hwnd) : PostprocessingShader(info, hwnd, "post/directionalBlur_post") {
	CreateConstantBuffer<DirectionalBlurBuffer, PIXEL_SHADER>(2);
}

void DirectionalBlurShader::UploadExtraSceneData() {
	PostprocessingShader::UploadExtraSceneData();

	auto& data = SceneDataAs<DirectionalBlurData>();

	DirectionalBlurBuffer buf{};
	buf.angle = data.angle;
	buf.distance = data.distance;
	buf.samples = data.samples;

	UploadConstantBuffer(buf);
}

std::unique_ptr<Shader::ISceneData> DirectionalBlurShader::DefaultSceneData() const {
	return std::make_unique<DirectionalBlurData>();
}