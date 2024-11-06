#include <scene/Camera.h>

Camera::Camera(float width, float height, float nearPlane, float farPlane, float fieldOfView) {
	m_width = width;
	m_height = height;
	m_near = nearPlane;
	m_far = farPlane;
	m_fov = fieldOfView;

	RecalculateViewMatrix();
	RecalculateProjectionMatrix();
}

XMMATRIX Camera::ViewMatrix() const { return m_viewMatrix; }
XMMATRIX Camera::ProjectionMatrix() const { return m_projMatrix; }

void Camera::SetViewport(const XMFLOAT2& v) { SetViewport(v.x, v.y); }
void Camera::SetViewport(float width, float height) {
	m_width = width;
	m_height = height;
	RecalculateProjectionMatrix();
}
std::pair<float, float> Camera::Viewport() const {
	return std::make_pair(m_width, m_height);
}

float Camera::NearPlane() const { return m_near; }
float Camera::FarPlane() const { return m_far; }
float Camera::FieldOfView() const { return m_fov; }

void Camera::SetNearPlane(float v) {
	m_near = v;
	RecalculateProjectionMatrix();
}
void Camera::SetFarPlane(float v) {
	m_far = v;
	RecalculateProjectionMatrix();
}
void Camera::SetFieldOfView(float v) {
	m_fov = v;
	RecalculateProjectionMatrix();
}

XMVECTOR Camera::CameraUpVector() const {
	XMFLOAT3 up = XMFLOAT3(0.f, 1.f, 0.f);
	return XMLoadFloat3(&up);
}

XMMATRIX Camera::__Internal_CalculateViewMatrix() const {
	XMVECTOR up, positionv, lookAt;
	up = CameraUpVector();
	XMFLOAT3 position = Position();
	positionv = XMLoadFloat3(&position);
	lookAt = XMVectorSet(0.0, 0.0, 1.0f, 1.0f);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	XMMATRIX rotationMatrix = RotationMatrix();

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = positionv + lookAt;

	// Finally create the view matrix from the three updated vectors.
	return XMMatrixLookAtLH(positionv, lookAt, up);
}

XMMATRIX Camera::__Internal_CalculateProjectionMatrix() const {
	return XMMatrixPerspectiveFovLH(m_fov, m_width / m_height, m_near, m_far);
}


void Camera::RecalculateViewMatrix() { m_viewMatrix = __Internal_CalculateViewMatrix(); }
void Camera::RecalculateProjectionMatrix() { m_projMatrix = __Internal_CalculateProjectionMatrix(); }



void Camera::SetPosition(XMFLOAT3 v) {
	ITransformed::SetPosition(v);
	RecalculateViewMatrix();
}
void Camera::SetRotation(XMFLOAT3 v) {
	ITransformed::SetRotation(v);
	RecalculateViewMatrix();
}
void Camera::SetPitch(float v) {
	ITransformed::SetPitch(v);
	RecalculateViewMatrix();
}
void Camera::SetYaw(float v) {
	ITransformed::SetYaw(v);
	RecalculateViewMatrix();
}
void Camera::SetRoll(float v) {
	ITransformed::SetRoll(v);
	RecalculateViewMatrix();
}