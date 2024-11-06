#pragma once

#include <shaders/BaseShader.h>

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