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
#include <util/TypeIndexUtil.h>

using namespace DirectX;

class BaseShader : public IDeviceContextDependent
{
public:
	BaseShader(const DeviceInfo&, HWND hwnd);

	virtual void Render(BaseMesh&);
	void Compute(int x, int y, int z);

protected:
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

	enum ShaderType { VERTEX_SHADER, PIXEL_SHADER, HULL_SHADER, DOMAIN_SHADER, GEOMETRY_SHADER, COMPUTE_SHADER };

protected:
	template<typename BufferType> void UploadConstantBuffer(const BufferType& bufferContents) {
		auto& info = m_constantBuffers.at(type_index<BufferType>());
		auto& buffer = info.UniqueResource();
		__Internal_UploadBuffer(buffer.get(), bufferContents);
		auto& slotsMap = info.SlotsMap();
		for (auto& pair : slotsMap) {
			ShaderType shaderType = pair.first;
			int slot = pair.second;
			if (slot >= 0) {
				switch (shaderType) {
				case VERTEX_SHADER:		{ p_deviceContext->VSSetConstantBuffers(slot, 1, buffer.ptr_to_ptr()); } break;
				case PIXEL_SHADER:		{ p_deviceContext->PSSetConstantBuffers(slot, 1, buffer.ptr_to_ptr()); } break;
				case HULL_SHADER:		{ p_deviceContext->HSSetConstantBuffers(slot, 1, buffer.ptr_to_ptr()); } break;
				case DOMAIN_SHADER:		{ p_deviceContext->DSSetConstantBuffers(slot, 1, buffer.ptr_to_ptr()); } break;
				case GEOMETRY_SHADER:	{ p_deviceContext->GSSetConstantBuffers(slot, 1, buffer.ptr_to_ptr()); } break;
				case COMPUTE_SHADER:	{ p_deviceContext->CSSetConstantBuffers(slot, 1, buffer.ptr_to_ptr()); } break;
				}
			}
		}
	}

	unique_resource<ID3D11SamplerState> CreateSamplerState(D3D11_FILTER filter = D3D11_FILTER_ANISOTROPIC, D3D11_COMPARISON_FUNC comparisonFunc = D3D11_COMPARISON_ALWAYS);

	template<typename BufferType> void CreateConstantBuffer() {
		auto resource = unique_resource<ID3D11Buffer>(__Internal_CreateBuffer<BufferType>(D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE));
		m_constantBuffers.emplace(type_index<BufferType>(), ConstBufferInfo(std::move(resource)));
	}

	template<typename BufferType, ShaderType Shader> void CreateConstantBuffer(int slot) {
		CreateConstantBuffer<BufferType>();
		SetConstantBufferSlot<BufferType, Shader>(slot);
	}

	template<typename BufferType> ID3D11Buffer* GetConstantBuffer() const { return m_constantBuffers.at(type_index<BufferType>()).Resource(); }
	template<typename BufferType> bool HasConstantBuffer() const { return m_constantBuffers.count(type_index<BufferType>()) > 0; }

	template<typename BufferType, ShaderType Shader> int GetConstantBufferSlot() const {
		auto& info = m_constantBuffers.at(type_index<BufferType>());
		return info.GetSlot<ShaderType>();
	}

	template<typename BufferType, ShaderType Shader> void SetConstantBufferSlot(int slot) {
		auto& info = m_constantBuffers.at(type_index<BufferType>());
		if (slot < 0) { info.ClearSlot<Shader>(); }
		else {
			// Assert that slot is not already in use
			for (auto& pair : m_constantBuffers) { assert(slot != pair.second.GetSlot<Shader>()); }
			info.SetSlot<Shader>(slot);
		}
	}

	template<typename BufferType, ShaderType Shader> void ClearConstantBufferSlot() {
		auto& info = m_constantBuffers.at(type_index<BufferType>());
		info.ClearSlot<Shader>();
	}

private:
	template<typename BufferType> void __Internal_UploadBuffer(ID3D11Buffer* buffer, const BufferType& bufferContents) {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = p_deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &bufferContents, sizeof(BufferType));
		p_deviceContext->Unmap(buffer, 0);
	}

	template<typename BufferType> ID3D11Buffer* __Internal_CreateBuffer(D3D11_BIND_FLAG bind, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_CPU_ACCESS_FLAG cpuAccess = D3D11_CPU_ACCESS_WRITE) {
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
	class ConstBufferInfo {
	public:
		ConstBufferInfo(unique_resource<ID3D11Buffer>&& resource);

		ID3D11Buffer* Resource() const;
		unique_resource<ID3D11Buffer>& UniqueResource();

		template<ShaderType Shader> int GetSlot() const {
			try								 { return m_slots.at(Shader); }
			catch (const std::out_of_range&) { return -1; }
		}

		template<ShaderType Shader> void SetSlot(int slot)	{ m_slots[Shader] = slot; }
		template<ShaderType Shader> void ClearSlot()		{ m_slots.erase(Shader); }

		const std::map<ShaderType, int>& SlotsMap() const;

	private:
		unique_resource<ID3D11Buffer> pm_resource;
		std::map<ShaderType, int> m_slots;
	};

	std::map<std::type_index, ConstBufferInfo> m_constantBuffers;
};