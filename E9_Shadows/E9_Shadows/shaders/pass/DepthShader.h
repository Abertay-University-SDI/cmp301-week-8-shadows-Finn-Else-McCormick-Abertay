#pragma once

#include <shaders/BaseShader.h>
#include <shaders/ShaderInterfaces.h>

class DepthShader : public BaseShader, public IMatrixShader
{
public:
	DepthShader(const DeviceInfo&, HWND hwnd);

	virtual void UploadMatrixData(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection);
};