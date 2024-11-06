#include "PostprocessingShader.h"

PostprocessingShader::PostprocessingShader(const DeviceInfo& info, HWND hwnd, const std::string& pixelShader) : BaseShader(info, hwnd) {
	LoadVertexShader("shaders/post/postprocessing_vs.cso");
	LoadPixelShader("shaders/" + pixelShader + ".cso");

	CreateConstantBuffer<MatrixBuffer, VERTEX_SHADER>(0);
	CreateConstantBuffer<ScreenBuffer, PIXEL_SHADER>(1);

	sampleState = CreateSamplerState();
}

void PostprocessingShader::UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) {
	MatrixBuffer bufObject{ XMMatrixTranspose(world), XMMatrixTranspose(view), XMMatrixTranspose(projection) };
	UploadConstantBuffer(bufObject);
}

void PostprocessingShader::UploadTextureData(ID3D11ShaderResourceView* albedoTexture, ID3D11ShaderResourceView*) {
	// Set shader texture resource in the pixel shader.
	if (albedoTexture) { p_deviceContext->PSSetShaderResources(0, 1, &albedoTexture); }
	p_deviceContext->PSSetSamplers(0, 1, sampleState.ptr_to_ptr());
}

void PostprocessingShader::UploadModelData(const IModelData& rawData) {
	auto& data = dynamic_cast<const ScreenData&>(rawData);

	ScreenBuffer buf{};
	buf.width = data.width;
	buf.height = data.height;

	UploadConstantBuffer(buf);
}

std::unique_ptr<IModelDataShader::IModelData> PostprocessingShader::DefaultModelData() const {
	return std::make_unique<ScreenData>();
}