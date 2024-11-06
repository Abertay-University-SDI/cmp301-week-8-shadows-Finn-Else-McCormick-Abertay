/**
* \class Base Shader
*
* \brief Loads shaders CSOs and sets parameters
*
* Base shader class to be inherited. Provides default functionality of loading and sending to GPU
* TODO: Add compute shader to set
* Base shader is the parent for other custom shader objects. Offers required functions and a standard Matrix buffer.
* 
* \author Paul Robertson
*/

#pragma once

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <fstream>
#include "imGUI/imgui.h"

#include <system/DeviceContextDependent.h>
#include <geometry/BaseMesh.h>
#include <system/UniqueResource.h>

#include <cassert>
#include <map>
#include <typeindex>
#include <typeinfo>

using namespace std;
using namespace DirectX;


class BaseShader : public IDeviceContextDependent
{
public:
	BaseShader(const DeviceInfo&, HWND hwnd);

	virtual void Render(BaseMesh&);
	void Compute(int x, int y, int z);

protected:
	virtual void InitShader(const std::string&, const std::string&) = 0;
	void LoadVertexShader(const std::string& filename);				///< Load Vertex shader, for stand position, tex, normal geomtry
	void LoadColourVertexShader(const std::string& filename);		///< Load Vertex shader, pre-made for position and colour only
	void LoadTextureVertexShader(const std::string& filename);		///< Load Vertex shader, pre-made for position and tex only
	void LoadHullShader(const std::string& filename);				///< Load Hull shader
	void LoadDomainShader(const std::string& filename);				///< Load Domain shader
	void LoadGeometryShader(const std::string& filename);			///< Load Geometry shader
	void LoadPixelShader(const std::string& filename);				///< Load Pixel shader
	void LoadComputeShader(const std::string& filename);			///< Load computer shader

private:
	unique_resource<ID3DBlob> LoadFileToBlob(const std::string& filename, const std::string& shaderTypeName = "") const;

protected:
	HWND hwnd;
	
	unique_resource<ID3D11VertexShader> vertexShader;
	unique_resource<ID3D11PixelShader> pixelShader;
	unique_resource<ID3D11HullShader> hullShader;
	unique_resource<ID3D11DomainShader> domainShader;
	unique_resource<ID3D11GeometryShader> geometryShader;
	unique_resource<ID3D11ComputeShader> computeShader;
	unique_resource<ID3D11InputLayout> layout;

protected:
	template<typename BufferType>
	void UploadConstantBuffer(const BufferType& bufferContents) {
		auto& info = m_constantBuffers.at(std::type_index(typeid(BufferType)));
		auto& buffer = info.resource;
		__Internal_UploadBuffer(buffer.get(), bufferContents);
		if (info.pixelShaderSlot >= 0) {
			p_deviceContext->PSSetConstantBuffers(info.pixelShaderSlot, 1, buffer.ptr_to_ptr());
		}
		if (info.vertexShaderSlot >= 0) {
			p_deviceContext->VSSetConstantBuffers(info.vertexShaderSlot, 1, buffer.ptr_to_ptr());
		}
	}

	unique_resource<ID3D11SamplerState> CreateSamplerState(D3D11_FILTER filter = D3D11_FILTER_ANISOTROPIC, D3D11_COMPARISON_FUNC comparisonFunc = D3D11_COMPARISON_ALWAYS);

	template<typename BufferType>
	void CreateConstantBuffer(int pixelSlot = -1, int vertexSlot = -1) {
		for (auto& pair : m_constantBuffers) {
			auto& info = pair.second;
			assert(pixelSlot < 0 || pixelSlot != info.pixelShaderSlot);
			assert(vertexSlot < 0 || vertexSlot != info.vertexShaderSlot);
		}
		auto resource = unique_resource<ID3D11Buffer>(__Internal_CreateBuffer<BufferType>(D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE));
		ConstBufferInfo info = ConstBufferInfo(std::move(resource), pixelSlot, vertexSlot);
		m_constantBuffers.emplace(std::type_index(typeid(BufferType)), std::move(info));
	}

	template<typename BufferType>
	ID3D11Buffer* GetConstantBuffer() const {
		return m_constantBuffers.at(std::type_index(typeid(BufferType))).resource.get();
	}

	template<typename BufferType>
	bool HasConstantBuffer() const {
		return m_constantBuffers.count(std::type_index(typeid(BufferType))) > 0;
	}

	template<typename BufferType>
	void SetConstantBufferSlots(int pixelSlot, int vertexSlot) {
		auto& info = m_constantBuffers.at(std::type_index(typeid(BufferType)));
		info.pixelShaderSlot = pixelSlot;
		info.vertexShaderSlot = vertexSlot;
	}

private:
	template<typename BufferType>
	void __Internal_UploadBuffer(ID3D11Buffer* buffer, const BufferType& bufferContents) {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = p_deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &bufferContents, sizeof(BufferType));
		p_deviceContext->Unmap(buffer, 0);
	}

	template<typename BufferType>
	ID3D11Buffer* __Internal_CreateBuffer(D3D11_BIND_FLAG bind, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_CPU_ACCESS_FLAG cpuAccess = D3D11_CPU_ACCESS_WRITE) {
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.Usage = usage;
		bufferDesc.ByteWidth = sizeof(BufferType);
		bufferDesc.BindFlags = bind;
		bufferDesc.CPUAccessFlags = cpuAccess;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		ID3D11Buffer* buffer = nullptr;
		HRESULT result = p_device->CreateBuffer(&bufferDesc, NULL, &buffer);
		return buffer;
	}

private:
	struct ConstBufferInfo {
		unique_resource<ID3D11Buffer> resource;
		int pixelShaderSlot = -1;
		int vertexShaderSlot = -1;

		ConstBufferInfo(unique_resource<ID3D11Buffer>&& resource, int pixelSlot = -1, int vertexSlot = -1);
	};

	std::map<std::type_index, ConstBufferInfo> m_constantBuffers;
};