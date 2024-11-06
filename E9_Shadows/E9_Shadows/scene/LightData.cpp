#include "LightData.h"

XMFLOAT3 LightData::Diffuse() const { return m_buffer.diffuse; }
XMFLOAT3 LightData::Specular() const { return m_buffer.specular; }

void LightData::SetDiffuse(XMFLOAT3 c) { m_buffer.diffuse = c; }
void LightData::SetSpecular(XMFLOAT3 c) { m_buffer.specular = c; }

float LightData::Intensity() const { return m_buffer.intensity; }
float LightData::SpecularPower() const { return m_buffer.specularPower; }

void LightData::SetIntensity(float v) { m_buffer.intensity = v; }
void LightData::SetSpecularPower(float v) { m_buffer.specularPower = v; }

XMFLOAT3 LightData::DistanceFalloff() const { return m_buffer.distanceFalloff; }
float LightData::UmbraAngle() const { return m_buffer.umbraAngle; }
float LightData::PenumbraAngle() const { return m_buffer.penumbraAngle; }

void LightData::SetDistanceFalloff(XMFLOAT3 v) { m_buffer.distanceFalloff = v; }
void LightData::SetUmbraAngle(float v) { m_buffer.umbraAngle = v; }
void LightData::SetPenumbraAngle(float v) { m_buffer.penumbraAngle = v; }

bool LightData::IsDirectional() const { return m_directional; }

void LightData::SetIsDirectional(bool directional) {
	m_directional = directional;
	m_buffer.position.w = !m_directional;
}

const Transform& LightData::Trans() const { return m_transform; }

void LightData::SetTrans(const Transform& trans) {
	m_transform = trans;
	m_buffer.position = XMFLOAT4(m_transform.position.x, m_transform.position.y, m_transform.position.z, !m_directional);
	m_buffer.direction = m_transform.UpVector();
}

const LightData::Buffer& LightData::AsBuffer() const {
	return m_buffer;
}

LightData::LightData() : m_directional(false) {
	m_buffer.diffuse = XMFLOAT3(0.f, 0.f, 0.f);
	m_buffer.specular = XMFLOAT3(0.f, 0.f, 0.f);
	m_buffer.intensity = 1.f;
	m_buffer.specularPower = 32.f;
	m_buffer.position = XMFLOAT4(0.f, 0.f, 0.f, !m_directional);
	m_buffer.direction = XMFLOAT3(0.f, 1.f, 0.f);
	m_buffer.distanceFalloff = XMFLOAT3(0.f, 0.f, 1.f);
	m_buffer.umbraAngle = 0.f;
	m_buffer.penumbraAngle = XM_PI;
}