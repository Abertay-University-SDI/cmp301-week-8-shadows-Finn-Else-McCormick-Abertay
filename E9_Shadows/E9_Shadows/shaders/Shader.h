#pragma once

#include <shaders/BaseShader.h>
#include <scene/LightData.h>
#include <scene/Camera.h>

#include <shaders/ShaderInterfaces.h>

class Shader : public BaseShader
{
public:
	Shader(const DeviceInfo&, HWND hwnd, const std::string& vertexShader, const std::string& pixelShader);

	void UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection);

	void UploadTimeData(float time, float extraValue1 = 0.f, float extraValue2 = 0.f, float extraValue3 = 0.f);

	static const int NUM_LIGHTS = 8;
	void UploadLightData(const std::vector<LightData>&, const XMFLOAT3& ambient, const Camera&);

	void UploadTextureData(ID3D11ShaderResourceView* albedoTex, ID3D11ShaderResourceView* normalTex);

	struct IModelData { virtual ~IModelData() = default; }; virtual std::unique_ptr<IModelData> DefaultModelData() const;
	virtual void UploadExtraModelData(const IModelData&);

	struct ISceneData { virtual ~ISceneData() = default; };
	virtual void UploadExtraSceneData();

	ISceneData& SceneData();
	template <typename SceneDataType> SceneDataType& SceneDataAs() { return dynamic_cast<typename SceneDataType&>(SceneData()); }

protected:
	virtual std::unique_ptr<ISceneData> DefaultSceneData() const;

	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct TimeBuffer {
		float time;
		float extra1;
		float extra2;
		float extra3;
	};

	struct CameraBuffer {
		XMFLOAT3 position;
		float padding;
	};

	struct LightsBuffer {
		XMFLOAT3 ambientColour;
		float padding;
		LightData::Buffer lights[NUM_LIGHTS];
	};

private:
	unique_resource<ID3D11SamplerState> sampleState;

	std::unique_ptr<ISceneData> pm_sceneData;
};