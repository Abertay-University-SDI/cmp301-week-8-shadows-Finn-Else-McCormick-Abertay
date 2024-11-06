// PointMesh.h
// A simple mesh for demostrating the geometry shader
// Instead producing a triangle list, produce a point list.
// This point list is three points of a triangle and can be used by the geometry shader to generate geometry.
// Therefore has a modified sendData() function.

#pragma once

#include <geometry/BaseMesh.h>

using namespace DirectX;

class PointMesh : public BaseMesh
{

public:
	PointMesh(const DeviceInfo&);

	void SendData(D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST) override;

protected:
	void InitBuffers();

};