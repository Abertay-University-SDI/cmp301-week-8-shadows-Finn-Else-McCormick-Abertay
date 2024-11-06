// texture
// Loads and stores a single texture.
// Handles .dds, .png and .jpg (probably).
#include "TextureManager.h"

#include <util/WideStringUtils.h>


 //Attempt to load texture. If load fails use default texture.
 //Based on extension, uses slightly different loading function for different image types .dds vs .png/.jpg.
TextureManager::TextureManager(const DeviceInfo& info) : IDeviceContextDependent(info)
{
	AddDefaultTexture();
}

void TextureManager::Load(const std::string& uid, const std::string& filename)
{
	HRESULT result;

	// if not set default texture
	if (!Exists(filename))
	{
		// change default texture
		//filename = L"../res/DefaultDiffuse.png";
		MessageBox(NULL, L"Texture filename does not exist", L"ERROR", MB_OK);
		return;
	}

	// check file extension for correct loading function.
	std::wstring fn = to_wstring(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
	}

	unique_resource<ID3D11ShaderResourceView> shaderResourceView;
	// Load the texture in.
	if (extension == L"dds")
	{
		result = CreateDDSTextureFromFile(p_device, p_deviceContext, fn.c_str(), NULL, shaderResourceView.ptr_to_ptr());
	}
	else
	{
		result = CreateWICTextureFromFile(p_device, p_deviceContext, fn.c_str(), NULL, shaderResourceView.ptr_to_ptr(), 0);
	}
	
	if (FAILED(result))
	{
		MessageBox(NULL, L"Texture loading error", L"ERROR", MB_OK);
	}
	else
	{
		textureMap.emplace(uid, std::move(shaderResourceView));
	}
}

// Return texture as a shader resource.
ID3D11ShaderResourceView* TextureManager::Get(const std::string& uid)
{
	if (textureMap.find(uid) != textureMap.end())
	{
		// texture exists
		return textureMap.at(uid).get();
	}
	else
	{
		return GetDefault();
	}
}

ID3D11ShaderResourceView* TextureManager::GetDefault() {
	return textureMap.at("default").get();
}

bool TextureManager::Exists(const std::string& fname)
{
	std::ifstream infile(fname);
	return infile.good();
}

void TextureManager::AddDefaultTexture()
{
	
	static const uint32_t s_pixel = 0xffffffff;

	D3D11_SUBRESOURCE_DATA initData = { &s_pixel, sizeof(uint32_t), 0 };

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	unique_resource<ID3D11Texture2D> texture;
	HRESULT hr = p_device->CreateTexture2D(&desc, &initData, texture.ptr_to_ptr());

	if (SUCCEEDED(hr))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		unique_resource<ID3D11ShaderResourceView> shaderResourceView;
		hr = p_device->CreateShaderResourceView(texture.get(), &SRVDesc, shaderResourceView.ptr_to_ptr());
		textureMap.emplace("default", std::move(shaderResourceView));
	}
	
}