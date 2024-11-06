// plane mesh
// Quad mesh made of many quads. Default is 100x100
#include "planemesh.h"

// Initialise buffer and load texture.
PlaneMesh::PlaneMesh(const DeviceInfo& info, int resolution, bool doubleSided) : BaseMesh(info)
{
	m_resolution = resolution;
	m_doubleSided = doubleSided;
	InitBuffers();
}

// Release resources.
PlaneMesh::~PlaneMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Generate plane (including texture coordinates and normals).
void PlaneMesh::InitBuffers()
{	
	// Calculate the number of vertices in the terrain mesh.
	vertexCount = m_resolution * m_resolution * 4 * (m_doubleSided ? 2 : 1);
	indexCount = m_resolution * m_resolution * 6 * (m_doubleSided ? 2 : 1);
	VertexType* vertices = new VertexType[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];

	float increment = 1.0f / m_resolution;

	XMFLOAT3 normal = XMFLOAT3(0.f, 1.f, 0.f);

	for (int repetition = 0; repetition < (m_doubleSided ? 2 : 1); ++repetition) {
		for (int j = 0; j < (m_resolution); j++) {
			for (int i = 0; i < (m_resolution); i++) {
				auto genVert = [&](float u, float v) {
					return VertexType{ XMFLOAT3((i + u) * increment - 0.5f, 0.f, (j + v) * increment - 0.5f), XMFLOAT2((i + u) * increment, (j + v) * increment), normal};
				};

				int baseIndex = (j * m_resolution + i) * 4 + repetition * (m_resolution * m_resolution * 4);
				int indexLowerLeft  = baseIndex,
					indexUpperRight = baseIndex + 1,
					indexUpperLeft  = baseIndex + 2,
					indexLowerRight = baseIndex + 3;

				vertices[indexLowerLeft]  = genVert(0.f, 0.f);
				vertices[indexUpperRight] = genVert(1.f, 1.f);
				vertices[indexUpperLeft]  = genVert(0.f, 1.f);
				vertices[indexLowerRight] = genVert(1.f, 0.f);

				int indicesIndexStart = (j * m_resolution + i) * 6 + repetition * (m_resolution * m_resolution * 6);

				if (repetition == 0) {
					indices[indicesIndexStart + 0] = indexLowerLeft;
					indices[indicesIndexStart + 1] = indexUpperRight;
					indices[indicesIndexStart + 2] = indexUpperLeft;

					indices[indicesIndexStart + 3] = indexLowerLeft;
					indices[indicesIndexStart + 4] = indexLowerRight;
					indices[indicesIndexStart + 5] = indexUpperRight;
				}
				else {
					indices[indicesIndexStart + 0] = indexUpperLeft;
					indices[indicesIndexStart + 1] = indexUpperRight;
					indices[indicesIndexStart + 2] = indexLowerLeft;

					indices[indicesIndexStart + 3] = indexUpperRight;
					indices[indicesIndexStart + 4] = indexLowerRight;
					indices[indicesIndexStart + 5] = indexLowerLeft;
				}
			}
		}
		normal = XMFLOAT3(0.f, -1.f, 0.f);
	}

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	p_device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.ptr_to_ptr());
	
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	p_device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.ptr_to_ptr());
	
	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices; vertices = nullptr;
	delete[] indices; indices = nullptr;
}


