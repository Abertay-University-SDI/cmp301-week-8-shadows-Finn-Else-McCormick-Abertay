#pragma once

#include "Camera.h"

class OrthoCamera : public Camera {
public:
	OrthoCamera(float width, float height, float nearPlane, float farPlane, bool invertUp = false);

protected:
	virtual XMVECTOR CameraUpVector() const override;
	virtual XMMATRIX __Internal_CalculateProjectionMatrix() const override;

	bool m_invertUp;
};