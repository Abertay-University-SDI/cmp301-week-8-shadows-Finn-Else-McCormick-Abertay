#pragma once

#include <shaders/BaseShader.h>
#include <shaders/ShaderInterfaces.h>

class PostprocessingShader : public BaseShader, public IMatrixShader, public IModelDataShader, public ITextureShader
{
public:
	PostprocessingShader(const DeviceInfo&, HWND hwnd, const std::string& pixelShader = "post/default_post");

	virtual void UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) override;
	virtual void UploadTextureData(ID3D11ShaderResourceView* albedoTex, ID3D11ShaderResourceView* normalTex = nullptr) override;

	virtual void UploadModelData(const IModelData&) override;
	virtual std::unique_ptr<IModelData> DefaultModelData() const override;

	struct ScreenData : IModelDataShader::IModelData {
		float width;
		float height;
	};

private:
	struct ScreenBuffer {
		float width;
		float height;
		float _padding[2];
	};

	unique_resource<ID3D11SamplerState> sampleState;
};