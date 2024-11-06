#pragma once
// Texture
// Loads and stores a texture ready for rendering.
// Handles mipmap generation on load.

#pragma once

#include <d3d11.h>
#include "DTK\include\DDSTextureLoader.h"
#include "DTK\include\WICTextureLoader.h"
#include <string>
#include <fstream>
#include <map>

#include <system/DeviceContextDependent.h>
#include <system/UniqueResource.h>

using namespace DirectX;

class TextureManager : public IDeviceContextDependent
{
public:
	TextureManager(const DeviceInfo&);

	void Load(const std::string& uid, const std::string& filename);
	ID3D11ShaderResourceView* Get(const std::string&);
	ID3D11ShaderResourceView* GetDefault();

private:
	bool Exists(const std::string& fileName);
	void AddDefaultTexture();

	std::map<std::string, unique_resource<ID3D11ShaderResourceView>> textureMap;
};