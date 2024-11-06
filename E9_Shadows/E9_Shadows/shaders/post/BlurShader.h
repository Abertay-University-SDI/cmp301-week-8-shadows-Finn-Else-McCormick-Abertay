#pragma once

#include <shaders/post/PostprocessingShader.h>

#include <array>

class BlurShader : public PostprocessingShader
{
public:
	BlurShader(const DeviceInfo&, HWND hwnd, const std::string& pixelShader);

	static const int NUM_WEIGHTS = 5;
	struct BlurData : ISceneData {
		std::array<float, NUM_WEIGHTS> weights;
	};

	virtual void UploadExtraSceneData() override;

protected:
	virtual std::unique_ptr<ISceneData> DefaultSceneData() const override;

private:
	struct BlurBuffer {
		XMFLOAT4 weights[NUM_WEIGHTS];
	};
};