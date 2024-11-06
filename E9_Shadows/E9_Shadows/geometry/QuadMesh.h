// Quad Mesh
// Screenspace quad - only position and texture coordinates. Origin in top left corner

#pragma once

#include <geometry/BaseMesh.h>

using namespace DirectX;

class QuadMesh : public BaseMesh
{

public:
	QuadMesh(const DeviceInfo&);

protected:
	void InitBuffers();

	virtual void SendData(D3D_PRIMITIVE_TOPOLOGY top = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;
};