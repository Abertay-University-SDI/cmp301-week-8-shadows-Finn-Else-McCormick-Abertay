#include "ConversionFunctions.h"

using namespace DirectX;

ImVec2 ImVec(float x, float y) {
	return ImVec2(x, y);
}
ImVec2 ImVec(const DirectX::XMFLOAT2& v) {
	return ImVec2(v.x, v.y);
}
ImVec2 ImVec(const DirectX::XMVECTOR& v) {
	return ImVec(FLOAT2(v));
}

DirectX::XMFLOAT2 XMFLOAT(float x, float y) {
	return XMFLOAT2(x, y);
}
DirectX::XMFLOAT2 XMFLOAT(const ImVec2& v) {
	return XMFLOAT2(v.x, v.y);
}

DirectX::XMFLOAT2 FLOAT2(const DirectX::XMVECTOR& v) {
	XMFLOAT2 temp;
	XMStoreFloat2(&temp, v);
	return temp;
}
DirectX::XMFLOAT3 FLOAT3(const DirectX::XMVECTOR& v) {
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, v);
	return temp;
}
DirectX::XMFLOAT4 FLOAT4(const DirectX::XMVECTOR& v) {
	XMFLOAT4 temp;
	XMStoreFloat4(&temp, v);
	return temp;
}