// Sphere Mesh
// Generated sphere mesh with texture coordinates and normals.
// Uses the cube sphere normalisation method. First a cube is generated,
// then the vertices are normalised creating a sphere.
// Resolution specifies the number of segments in the sphere (top and bottom, matches equator).

#pragma once

#include <geometry/BaseMesh.h>

using namespace DirectX;

class SphereMesh : public BaseMesh
{

public:
	SphereMesh(const DeviceInfo&, int resolution = 20);

protected:
	void InitBuffers();
	int resolution;
};