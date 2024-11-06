#pragma once

#include <shaders/post/PostprocessingShader.h>

class DirectionalBlurShader : public PostprocessingShader
{
public:
	DirectionalBlurShader(const DeviceInfo&, HWND hwnd);

	struct DirectionalBlurData : ISceneData {
		float angle;
		float distance;
		int samples = 5;
	};

	virtual void UploadExtraSceneData() override;

protected:
	virtual std::unique_ptr<ISceneData> DefaultSceneData() const override;

private:
	struct DirectionalBlurBuffer {
		float angle;
		float distance;
		int samples;
		float padding;
	};
};