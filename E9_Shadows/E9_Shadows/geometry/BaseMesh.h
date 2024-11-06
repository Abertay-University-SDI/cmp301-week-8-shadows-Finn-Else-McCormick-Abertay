/**
* \class Base Mesh
*
* \brief The parent for all mesh objects. Provides default functionality.
*
* Can be inherited to create custom meshes. Provide functions for sending data to GPU memory, getting index count and storing geometry data.
*
* \author Paul Robertson
*/

#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include <system/DeviceContextDependent.h>
#include <system/UniqueResource.h>

using namespace DirectX;

class BaseMesh : public IDeviceContextDependent
{
protected:
	/// Default struct for general vertex data include position, texture coordinates and normals
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	/// Default vertex struct for geometry with only position and texture coordinates.
	struct VertexType_Texture
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	/// Transfers mesh data to the GPU.
	virtual void SendData(D3D_PRIMITIVE_TOPOLOGY top = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	int IndexCount() const;			///< Returns total index value of the mesh

protected:
	using IDeviceContextDependent::IDeviceContextDependent;
	virtual void InitBuffers() = 0;

	unique_resource<ID3D11Buffer> vertexBuffer, indexBuffer;
	int vertexCount, indexCount;
};