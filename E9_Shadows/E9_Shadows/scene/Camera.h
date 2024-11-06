#pragma once

#include <directxmath.h>
#include <utility>
#include <Transform.h>

class Camera : public ITransformed {
public:
	Camera(float width, float height, float nearPlane, float farPlane, float fieldOfView = XM_PI / 4.f);

	XMMATRIX ViewMatrix() const;
	XMMATRIX ProjectionMatrix() const;
		
	std::pair<float,float> Viewport() const;
	void SetViewport(float width, float height); void SetViewport(const XMFLOAT2&);

	float NearPlane() const; float FarPlane() const; float FieldOfView() const;
	void SetNearPlane(float); void SetFarPlane(float); void SetFieldOfView(float);

	void RecalculateViewMatrix();
	void RecalculateProjectionMatrix();

	virtual void SetPosition(XMFLOAT3) override; using ITransformed::SetPosition;
	virtual void SetRotation(XMFLOAT3) override; using ITransformed::SetRotation;
	virtual void SetPitch(float) override;
	virtual void SetYaw(float) override;
	virtual void SetRoll(float) override;

protected:
	virtual XMVECTOR CameraUpVector() const;

	virtual XMMATRIX __Internal_CalculateViewMatrix() const;
	virtual XMMATRIX __Internal_CalculateProjectionMatrix() const;

	float m_width, m_height;
	float m_near, m_far, m_fov;

private:
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projMatrix;
};