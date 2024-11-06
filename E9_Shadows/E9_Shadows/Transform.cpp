#include "Transform.h"

#include <utility>

Transform::Transform() : position(0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f), rotation(0.f, 0.f, 0.f) {}
Transform::Transform(XMFLOAT2 pos, XMFLOAT2 rot) : Transform(std::move(pos), 1.f, std::move(rot)) {}
Transform::Transform(XMFLOAT2 pos, float scale, XMFLOAT2 rot) : Transform(std::move(pos), XMFLOAT2(scale, scale), std::move(rot)) {}
Transform::Transform(XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT2 rot) : Transform(XMFLOAT3(pos.x, pos.y, 0.f), XMFLOAT3(scale.x, scale.y, 1.f), XMFLOAT3(rot.x, rot.y, 0.f)) {}

Transform::Transform(XMFLOAT3 pos, XMFLOAT3 rot) : Transform(std::move(pos), 1.f, std::move(rot)) {}
Transform::Transform(XMFLOAT3 pos, float scale, XMFLOAT3 rot) : Transform(std::move(pos), XMFLOAT3(scale, scale, scale), std::move(rot)) {}
Transform::Transform(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rot) : position(std::move(pos)), scale(std::move(scale)), rotation(std::move(rot)) {}

void Transform::SetPosition(XMFLOAT2 pos) { this->position = XMFLOAT3(pos.x, pos.y, 0.f); }
void Transform::SetPosition(XMFLOAT3 pos) { this->position = pos; }
void Transform::SetScale(float scale) { this->scale = XMFLOAT3(scale, scale, scale); }
void Transform::SetScale(XMFLOAT2 scale) { this->scale = XMFLOAT3(scale.x, scale.y, 1.f); }
void Transform::SetScale(XMFLOAT3 scale) { this->scale = scale; }
void Transform::SetRotation(XMFLOAT3 rot) { this->rotation = rot; }
void Transform::SetPitch(float v) { this->rotation.x = v; }
void Transform::SetYaw(float v) { this->rotation.y = v; }
void Transform::SetRoll(float v) { this->rotation.z = v; }

XMMATRIX Transform::AsMatrix() const {
	return XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		XMMatrixTranslation(position.x, position.y, position.z);
}

XMFLOAT3 Transform::UpVector() const {
	return Rotated(XMFLOAT3(0.f, 1.f, 0.f));
}

XMFLOAT3 Transform::ForwardVector() const {
	return Rotated(XMFLOAT3(0.f, 0.f, 1.f));
}

XMFLOAT3 Transform::Rotated(XMFLOAT3 vec) const {
	XMVECTOR result = XMVector3Transform(XMLoadFloat3(&vec), XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
	XMFLOAT3 rotated; XMStoreFloat3(&rotated, result);
	return rotated;
}


ITransformed::ITransformed(const Transform& trans) : m_trans(trans) {}
ITransformed::ITransformed() {}

XMFLOAT3 ITransformed::Position() const { return m_trans.position; }
XMFLOAT3 ITransformed::Rotation() const { return m_trans.rotation; }
float ITransformed::Pitch() const { return m_trans.rotation.x; }
float ITransformed::Yaw() const { return m_trans.rotation.y; }
float ITransformed::Roll() const { return m_trans.rotation.z; }

void ITransformed::SetPosition(XMFLOAT3 v) { m_trans.SetPosition(v); }
void ITransformed::SetPosition(float x, float y, float z) { SetPosition(XMFLOAT3(x, y, z)); }

void ITransformed::SetRotation(XMFLOAT3 v) { m_trans.SetRotation(v); }
void ITransformed::SetRotation(float p, float y, float r) { SetRotation(XMFLOAT3(p, y, r)); }

void ITransformed::SetPitch(float v) { m_trans.SetPitch(v); }
void ITransformed::SetYaw(float v) { m_trans.SetYaw(v); }
void ITransformed::SetRoll(float v) { m_trans.SetRoll(v); }

void ITransformed::SetScale(XMFLOAT3 v) { m_trans.SetScale(v); }
void ITransformed::SetScale(float x, float y, float z) { SetScale(XMFLOAT3(x,y,z)); }
void ITransformed::SetScale(float v) { SetScale(XMFLOAT3(v,v,v)); }

XMMATRIX ITransformed::TranslationMatrix() const { return XMMatrixTranslation(m_trans.position.x, m_trans.position.y, m_trans.position.z); }
XMMATRIX ITransformed::RotationMatrix() const { return XMMatrixRotationRollPitchYaw(m_trans.rotation.x, m_trans.rotation.y, m_trans.rotation.z); }
XMMATRIX ITransformed::ScaleMatrix() const { return XMMatrixScaling(m_trans.scale.x, m_trans.scale.y, m_trans.scale.z); }
XMMATRIX ITransformed::TransformationMatrix() const {
	return ScaleMatrix() * RotationMatrix() * TranslationMatrix();
}

XMFLOAT3 ITransformed::UpVector() const {
	return m_trans.UpVector();
}

XMFLOAT3 ITransformed::ForwardVector() const {
	return m_trans.ForwardVector();
}

Transform& ITransformed::Trans() { return m_trans; }