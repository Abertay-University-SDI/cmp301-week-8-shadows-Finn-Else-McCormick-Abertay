#include "DepthShader.h"

DepthShader::DepthShader(const DeviceInfo& info, HWND hwnd) : BaseShader(info, hwnd) {

	LoadVertexShader("shaders/pass/depth_vs.cso");
	LoadPixelShader("shaders/pass/depth_ps.cso");

	CreateConstantBuffer<MatrixBuffer, VERTEX_SHADER>(0);
}

void DepthShader::UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection) {
	MatrixBuffer bufObject{ XMMatrixTranspose(world), XMMatrixTranspose(view), XMMatrixTranspose(projection) };
	UploadConstantBuffer(bufObject);
}