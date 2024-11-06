#include "SceneShader.h"

#include <util/WideStringUtils.h>

SceneShader::SceneShader(const DeviceInfo& info, HWND hwnd, const std::string& vertexShader, const std::string& pixelShader) : BaseShader(info, hwnd)
{
	// Load (+ compile) shader files
	LoadVertexShader("shaders/" + vertexShader + ".cso");
	LoadPixelShader("shaders/" + pixelShader + ".cso");

	CreateConstantBuffer<MatrixBuffer, VERTEX_SHADER>(0);
	CreateConstantBuffer<LightsBuffer, PIXEL_SHADER>(1);
	CreateConstantBuffer<CameraBuffer, VERTEX_SHADER>(2);
	CreateConstantBuffer<TimeBuffer>(); SetConstantBufferSlot<TimeBuffer, PIXEL_SHADER>(0); SetConstantBufferSlot<TimeBuffer, VERTEX_SHADER>(1);

	sampleState = CreateSamplerState();
}

void SceneShader::UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) {
	MatrixBuffer bufObject{ XMMatrixTranspose(world), XMMatrixTranspose(view), XMMatrixTranspose(projection) };
	UploadConstantBuffer(bufObject);
}

void SceneShader::UploadTimeData(float time, float extraValue1, float extraValue2, float extraValue3) {
	TimeBuffer bufObject{ time, extraValue1, extraValue2, extraValue3 };
	UploadConstantBuffer(bufObject);
}

void SceneShader::UploadLightData(const std::vector<LightData>& lights, const XMFLOAT3& ambient, const Camera& camera)
{
	// Send light data to pixel shader
	LightsBuffer lightsBufObject{};
	lightsBufObject.ambientColour = ambient;
	lightsBufObject.padding = 0;

	size_t numLights = (lights.size() < NUM_LIGHTS) ? lights.size() : NUM_LIGHTS;
	for (size_t i = 0; i < numLights; ++i) {
		lightsBufObject.lights[i] = lights.at(i).AsBuffer();
	}
	UploadConstantBuffer(lightsBufObject);

	// Send camera data to vertex shader
	CameraBuffer camBufObject{ camera.Position(), 0.f };
	UploadConstantBuffer(camBufObject);
}

void SceneShader::UploadTextureData(ID3D11ShaderResourceView* albedoTexture, ID3D11ShaderResourceView* normalTexture) {
	// Set shader texture resource in the pixel shader.
	if (albedoTexture) { p_deviceContext->PSSetShaderResources(0, 1, &albedoTexture); }
	if (normalTexture) { p_deviceContext->PSSetShaderResources(1, 1, &normalTexture); }
	p_deviceContext->PSSetSamplers(0, 1, sampleState.ptr_to_ptr());
}