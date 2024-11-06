#include "DeviceContextDependent.h"

IDeviceContextDependent::IDeviceContextDependent(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	SetContext(device, deviceContext);
}

IDeviceContextDependent::IDeviceContextDependent(const DeviceInfo& info) {
	SetContext(info);
}

void IDeviceContextDependent::SetContext(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	p_device = device;
	p_deviceContext = deviceContext;
}

void IDeviceContextDependent::SetContext(const DeviceInfo& info) {
	SetContext(info.device, info.context);
}