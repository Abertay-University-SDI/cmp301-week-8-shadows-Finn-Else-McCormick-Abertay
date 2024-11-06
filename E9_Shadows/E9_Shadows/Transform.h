#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct Transform {
	Transform();
	Transform(XMFLOAT2 pos, XMFLOAT2 rot = XMFLOAT2());
	Transform(XMFLOAT2 pos, float scale, XMFLOAT2 rot = XMFLOAT2());
	Transform(XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT2 rot);
	Transform(XMFLOAT3 pos, XMFLOAT3 rot = XMFLOAT3());
	Transform(XMFLOAT3 pos, float scale, XMFLOAT3 rot = XMFLOAT3());
	Transform(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rot);
	Transform(Transform&) = default;
	Transform(const Transform&) = default;

	XMFLOAT3 position;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;

	void SetPosition(XMFLOAT2); void SetPosition(XMFLOAT3);
	void SetScale(float); void SetScale(XMFLOAT2); void SetScale(XMFLOAT3);
	void SetRotation(XMFLOAT3);
	void SetPitch(float); void SetYaw(float); void SetRoll(float);

	XMMATRIX AsMatrix() const;

	XMFLOAT3 UpVector() const;
	XMFLOAT3 ForwardVector() const;

	XMFLOAT3 Rotated(XMFLOAT3) const;
};

class ITransformed {
public:
	XMFLOAT3 Position() const;
	XMFLOAT3 Rotation() const;
	float Pitch() const; float Yaw() const; float Roll() const;

	virtual void SetPosition(XMFLOAT3); void SetPosition(float, float, float);
	virtual void SetRotation(XMFLOAT3); void SetRotation(float, float, float);
	virtual void SetPitch(float); virtual void SetYaw(float); virtual void SetRoll(float);
	virtual void SetScale(XMFLOAT3); void SetScale(float, float, float); void SetScale(float);

	virtual XMMATRIX TranslationMatrix() const;
	virtual XMMATRIX RotationMatrix() const;
	virtual XMMATRIX ScaleMatrix() const;
	XMMATRIX TransformationMatrix() const;

	XMFLOAT3 UpVector() const;
	XMFLOAT3 ForwardVector() const;

	Transform& Trans();

protected:
	ITransformed(const Transform&);
	ITransformed();

	Transform m_trans;
};