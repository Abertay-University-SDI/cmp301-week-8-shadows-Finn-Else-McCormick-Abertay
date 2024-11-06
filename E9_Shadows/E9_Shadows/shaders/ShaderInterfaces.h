#pragma once

#include <shaders/BaseShader.h>
#include <scene/LightData.h>
#include <scene/Camera.h>

class IMatrixShader
{
public:
	virtual void UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) = 0;

protected:
	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
};

class ILightShader
{
public:
	static const int NUM_LIGHTS = 8;
	virtual void UploadLightData(const std::vector<LightData>&, const XMFLOAT3& ambient, const Camera&) = 0;

protected:
	struct CameraBuffer {
		XMFLOAT3 position;
		float padding;
	};

	struct LightsBuffer {
		XMFLOAT3 ambientColour;
		float padding;
		LightData::Buffer lights[NUM_LIGHTS];
	};
};


class ITimeShader
{
public:
	virtual void UploadTimeData(float time, float extraValue1 = 0.f, float extraValue2 = 0.f, float extraValue3 = 0.f) = 0;

protected:
	struct TimeBuffer {
		float time;
		float extra1;
		float extra2;
		float extra3;
	};
};

class ITextureShader
{
public:
	virtual void UploadTextureData(ID3D11ShaderResourceView* albedoTex, ID3D11ShaderResourceView* normalTex) = 0;
};

class ISceneDataShader
{
public:
	struct ISceneData { virtual ~ISceneData() = default; };
	void UploadSceneData();

	ISceneData& SceneData();
	template <typename SceneDataType> SceneDataType& SceneDataAs() { return dynamic_cast<SceneDataType&>(SceneData()); }

protected:
	virtual std::unique_ptr<ISceneData> DefaultSceneData() const;
	virtual void __Internal_UploadSceneData(const ISceneData&) = 0;

private:
	std::unique_ptr<ISceneData> pm_sceneData;
};

class IModelDataShader
{
public:
	struct IModelData { virtual ~IModelData() = default; };
	
	virtual std::unique_ptr<IModelData> DefaultModelData() const = 0;
	virtual void UploadModelData(const IModelData&) = 0;
};