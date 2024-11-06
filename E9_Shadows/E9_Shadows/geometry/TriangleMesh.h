#pragma once

#include <geometry/BaseMesh.h>

using namespace DirectX;

class TriangleMesh : public BaseMesh
{

public:
	TriangleMesh(const DeviceInfo&);

protected:
	void InitBuffers();
	
};