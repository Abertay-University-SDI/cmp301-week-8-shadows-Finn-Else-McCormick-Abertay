#pragma once

#include "Transform.h"
#include "Light.h"
#include <memory>
#include <vector>

class LightData
{
public:
	LightData();

	struct Buffer {
		XMFLOAT3 diffuse;
		float intensity;
		XMFLOAT3 specular;
		float specularPower;
		XMFLOAT4 position;
		XMFLOAT3 direction;
		float umbraAngle;
		XMFLOAT3 distanceFalloff;
		float penumbraAngle;
	};

	XMFLOAT3 Diffuse() const; void SetDiffuse(XMFLOAT3);
	XMFLOAT3 Specular() const; void SetSpecular(XMFLOAT3);

	float Intensity() const; void SetIntensity(float);
	float SpecularPower() const; void SetSpecularPower(float);

	XMFLOAT3 DistanceFalloff() const; void SetDistanceFalloff(XMFLOAT3);
	float UmbraAngle() const; void SetUmbraAngle(float);
	float PenumbraAngle() const; void SetPenumbraAngle(float);

	bool IsDirectional() const; void SetIsDirectional(bool);

	const Transform& Trans() const;
	void SetTrans(const Transform&);

	const LightData::Buffer& AsBuffer() const;

protected:
	Buffer m_buffer;
	Transform m_transform;
	bool m_directional;
};