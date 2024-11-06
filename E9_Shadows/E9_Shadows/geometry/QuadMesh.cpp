#include "quadmesh.h"

#include <vector>

// Initialise buffers and lad texture.
QuadMesh::QuadMesh(const DeviceInfo& info) : BaseMesh(info)
{
	InitBuffers();

}

// Build quad mesh.
void QuadMesh::InitBuffers()
{	
	this->vertexCount = 4;
	this->indexCount = 6;

	std::vector<VertexType_Texture> vertices; vertices.reserve(vertexCount);
	std::vector<unsigned long> indices; indices.reserve(indexCount);

	// Load the vertex array with data.
	vertices.push_back(VertexType_Texture{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }); // Top left

	vertices.push_back(VertexType_Texture{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }); // Top right

	vertices.push_back(VertexType_Texture{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }); // Bottom left

	vertices.push_back(VertexType_Texture{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }); // Bottom right

	// Load the index array with data.
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(0);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(1);

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType_Texture)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData{};
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	p_device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.ptr_to_ptr());
	
	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	p_device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.ptr_to_ptr());
}

void QuadMesh::SendData(D3D_PRIMITIVE_TOPOLOGY top) {
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType_Texture);
	offset = 0;

	p_deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.ptr_to_ptr(), &stride, &offset);
	p_deviceContext->IASetIndexBuffer(indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);
	p_deviceContext->IASetPrimitiveTopology(top);
}