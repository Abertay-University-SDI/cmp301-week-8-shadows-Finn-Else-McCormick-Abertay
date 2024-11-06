#include "BlurShader.h"

BlurShader::BlurShader(const DeviceInfo& info, HWND hwnd, const std::string& pixelShader) : PostprocessingShader(info, hwnd, pixelShader) {
	CreateConstantBuffer<BlurBuffer, PIXEL_SHADER>(2);
}

void BlurShader::UploadExtraSceneData() {
	PostprocessingShader::UploadExtraSceneData();

	auto& data = SceneDataAs<BlurData>();

	BlurBuffer buf{};
	for (int i = 0; i < NUM_WEIGHTS; ++i) {
		buf.weights[i].x = data.weights.at(i);
	}

	UploadConstantBuffer(buf);
}

std::unique_ptr<Shader::ISceneData> BlurShader::DefaultSceneData() const {
	return std::make_unique<BlurData>();
}