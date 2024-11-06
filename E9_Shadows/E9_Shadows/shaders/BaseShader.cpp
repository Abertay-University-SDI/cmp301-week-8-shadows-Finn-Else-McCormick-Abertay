// base shader
// Base class for shader object. Handles loading in shader files (vertex, pixel, domain, hull and geometry).
// Handle render/sending to GPU for processing.
#include "BaseShader.h"

#include <util/WideStringUtils.h>

// Store pointer to render device and handle to window.
BaseShader::BaseShader(const DeviceInfo& info, HWND lhwnd) : IDeviceContextDependent(info)
{
	hwnd = hwnd;
}

unique_resource<ID3DBlob> BaseShader::LoadFileToBlob(const std::string& filename, const std::string& shaderTypeArg) const {
	std::string shaderType = shaderTypeArg + (shaderTypeArg != "" ? " " : "");

	// check file extension
	auto extensionStartIndex = filename.rfind('.');
	if (extensionStartIndex == std::string::npos) {
		MessageBox(hwnd, to_wstring("Error finding " + shaderType + "shader file").c_str(), L"ERROR", MB_OK);
		exit(0);
	}
	std::string extension = filename.substr(extensionStartIndex + 1);
	if (extension != "cso") {
		MessageBox(hwnd, to_wstring("Incorrect " + shaderType + "shader file type").c_str(), L"ERROR", MB_OK);
		exit(0);
	}

	unique_resource<ID3DBlob> buffer;
	std::wstring filenameWin = to_wstring(filename);

	HRESULT result = D3DReadFileToBlob(filenameWin.c_str(), buffer.ptr_to_ptr());
	if (result != S_OK) {
		MessageBox(NULL, filenameWin.c_str(), L"File ERROR", MB_OK);
		exit(0);
	}

	return buffer;
}

void BaseShader::LoadVertexShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "vertex");
	
	// Create the vertex shader from the buffer.
	p_device->CreateVertexShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, vertexShader.ptr_to_ptr());
	
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the MeshClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	p_device->CreateInputLayout(polygonLayout, numElements, fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), layout.ptr_to_ptr());
}

void BaseShader::LoadTextureVertexShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "vertex");

	// Create the vertex shader from the buffer.
	p_device->CreateVertexShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, vertexShader.ptr_to_ptr());

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the MeshClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	p_device->CreateInputLayout(polygonLayout, numElements, fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), layout.ptr_to_ptr());
}

void BaseShader::LoadColourVertexShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "vertex");

	// Create the vertex shader from the buffer.
	p_device->CreateVertexShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, vertexShader.ptr_to_ptr());

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the MeshClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	p_device->CreateInputLayout(polygonLayout, numElements, fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), layout.ptr_to_ptr());
}


void BaseShader::LoadPixelShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "pixel");
	p_device->CreatePixelShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, pixelShader.ptr_to_ptr());
}

void BaseShader::LoadHullShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "hull");
	p_device->CreateHullShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, hullShader.ptr_to_ptr());
}

void BaseShader::LoadDomainShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "domain");
	p_device->CreateDomainShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, domainShader.ptr_to_ptr());
}

void BaseShader::LoadGeometryShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "geometry");
	p_device->CreateGeometryShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, geometryShader.ptr_to_ptr());
}

void BaseShader::LoadComputeShader(const std::string& filename)
{
	unique_resource<ID3DBlob> fileBuffer = LoadFileToBlob(filename, "compute");
	p_device->CreateComputeShader(fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), NULL, computeShader.ptr_to_ptr());
}

unique_resource<ID3D11SamplerState> BaseShader::CreateSamplerState(D3D11_FILTER filter, D3D11_COMPARISON_FUNC comparisonFunc) {
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = comparisonFunc;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* sampleState;
	p_device->CreateSamplerState(&samplerDesc, &sampleState);

	return unique_resource<ID3D11SamplerState>(sampleState);
}

BaseShader::ConstBufferInfo::ConstBufferInfo(unique_resource<ID3D11Buffer>&& resource) : pm_resource(std::move(resource)) {}

ID3D11Buffer* BaseShader::ConstBufferInfo::Resource() const {
	return pm_resource.get();
}

unique_resource<ID3D11Buffer>& BaseShader::ConstBufferInfo::UniqueResource() {
	return pm_resource;
}

const std::map<BaseShader::ShaderType, int>& BaseShader::ConstBufferInfo::SlotsMap() const {
	return m_slots;
}

// De/Activate shader stages and send shaders to GPU.
void BaseShader::Render(BaseMesh& mesh)
{
	mesh.SendData();

	// Set the vertex input layout.
	p_deviceContext->IASetInputLayout(layout.get());

	// Set the vertex and pixel shaders that will be used to render.
	p_deviceContext->VSSetShader(vertexShader.get(), NULL, 0);
	p_deviceContext->PSSetShader(pixelShader.get(), NULL, 0);
	p_deviceContext->CSSetShader(NULL, NULL, 0);
	
	// if Hull shader is not null then set HS and DS
	if (hullShader) {
		p_deviceContext->HSSetShader(hullShader.get(), NULL, 0);
		p_deviceContext->DSSetShader(domainShader.get(), NULL, 0);
	}
	else {
		p_deviceContext->HSSetShader(NULL, NULL, 0);
		p_deviceContext->DSSetShader(NULL, NULL, 0);
	}

	// if Geometry shader is not null then set GS
	if (geometryShader) {
		p_deviceContext->GSSetShader(geometryShader.get(), NULL, 0);
	}
	else {
		p_deviceContext->GSSetShader(NULL, NULL, 0);
	}

	// Render the triangle.
	p_deviceContext->DrawIndexed(mesh.IndexCount(), 0, 0);
}

// Dispatch the compute shader.
void BaseShader::Compute(int x, int y, int z)
{
	p_deviceContext->CSSetShader(computeShader.get(), NULL, 0);
	p_deviceContext->Dispatch(x, y, z);
}