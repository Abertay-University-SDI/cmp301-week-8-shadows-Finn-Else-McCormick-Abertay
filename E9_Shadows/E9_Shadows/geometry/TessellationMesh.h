// Tessellation Mesh
// A simple triangle mesh used for tessellation demonstration.
// Different output type, instead of Triangle list, a 3 control point patch list.
// The tessellator uses these points to generate new geometry but sub-division.
// Due to change in geometry type the sendData() function has been overridden.

#pragma once

#include <geometry/BaseMesh.h>

using namespace DirectX;

class TessellationMesh : public BaseMesh
{

public:
	TessellationMesh(const DeviceInfo&);

	void SendData(D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST) override;

protected:
	void InitBuffers();
	
};