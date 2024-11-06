#pragma once

#include <d3d11.h>

struct DeviceInfo {
	ID3D11Device* device;
	ID3D11DeviceContext* context;
};

class IDeviceContextDependent
{
public:
	void SetContext(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void SetContext(const DeviceInfo&);

protected:
	IDeviceContextDependent(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	IDeviceContextDependent(const DeviceInfo&);
	IDeviceContextDependent() = default;

	ID3D11Device* p_device;
	ID3D11DeviceContext* p_deviceContext;
};