#pragma once

#include <shaders/BaseShader.h>
#include <shaders/ShaderInterfaces.h>

class SceneShader : public BaseShader, public IMatrixShader, public ILightShader, public ITimeShader, public ITextureShader
{
public:
	SceneShader(const DeviceInfo&, HWND hwnd, const std::string& vertexShader, const std::string& pixelShader);

	virtual void UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) override;
	virtual void UploadLightData(const std::vector<LightData>&, const XMFLOAT3& ambient, const Camera&) override;
	virtual void UploadTimeData(float time, float extraValue1 = 0.f, float extraValue2 = 0.f, float extraValue3 = 0.f) override;
	virtual void UploadTextureData(ID3D11ShaderResourceView* albedoTex, ID3D11ShaderResourceView* normalTex) override;

private:
	unique_resource<ID3D11SamplerState> sampleState;
};