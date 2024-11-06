#pragma once

#include <shaders/post/PostprocessingShader.h>

class DirectionalBlurShader : public PostprocessingShader, public ISceneDataShader
{
public:
	DirectionalBlurShader(const DeviceInfo&, HWND hwnd);

	struct DirectionalBlurData : ISceneData {
		float angle;
		float distance;
		int samples = 5;
	};

protected:
	virtual std::unique_ptr<ISceneData> DefaultSceneData() const override;
	virtual void __Internal_UploadSceneData(const ISceneData&) override;

private:
	struct DirectionalBlurBuffer {
		float angle;
		float distance;
		int samples;
		float padding;
	};
};