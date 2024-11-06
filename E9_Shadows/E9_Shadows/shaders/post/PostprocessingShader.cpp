#include "PostprocessingShader.h"

PostprocessingShader::PostprocessingShader(const DeviceInfo& info, HWND hwnd, const std::string& pixelShader) : Shader(info, hwnd, "post/postprocessing_vs", pixelShader) {
	SetConstantBufferSlots<LightsBuffer>(-1, -1);
	SetConstantBufferSlots<CameraBuffer>(-1, -1);
	CreateConstantBuffer<ScreenBuffer>(1);
}

void PostprocessingShader::UploadExtraModelData(const IModelData& rawData) {
	auto& data = dynamic_cast<const ScreenData&>(rawData);

	ScreenBuffer buf{};
	buf.width = data.width;
	buf.height = data.height;

	UploadConstantBuffer(buf);
}

std::unique_ptr<Shader::IModelData> PostprocessingShader::DefaultModelData() const {
	return std::make_unique<ScreenData>();
}