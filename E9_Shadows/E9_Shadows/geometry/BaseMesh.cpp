// BaseMesh.cpp
// Base mesh class, for inheriting base mesh functionality.

#include "BaseMesh.h"

int BaseMesh::IndexCount() const
{
	return indexCount;
}

// Sends geometry data to the GPU. Default primitive topology is TriangleList.
// To render alternative topologies this function needs to be overwritten.
void BaseMesh::SendData(D3D_PRIMITIVE_TOPOLOGY top)
{
	unsigned int stride;
	unsigned int offset;
	
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	p_deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.ptr_to_ptr(), &stride, &offset);
	p_deviceContext->IASetIndexBuffer(indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);
	p_deviceContext->IASetPrimitiveTopology(top);
}




