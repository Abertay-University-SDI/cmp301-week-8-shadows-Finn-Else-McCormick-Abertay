#pragma once

#include <shaders/Shader.h>

class PostprocessingShader : public Shader
{
public:
	PostprocessingShader(const DeviceInfo&, HWND hwnd, const std::string& pixelShader = "post/default_post");

	struct ScreenData : Shader::IModelData {
		float width;
		float height;
	};

	virtual void UploadExtraModelData(const IModelData&) override;
	virtual std::unique_ptr<IModelData> DefaultModelData() const override;

private:
	struct ScreenBuffer {
		float width;
		float height;
		float _padding[2];
	};
};