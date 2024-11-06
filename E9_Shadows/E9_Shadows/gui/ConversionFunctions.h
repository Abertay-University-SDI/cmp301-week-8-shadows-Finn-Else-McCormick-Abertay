#pragma once

#include <DirectXMath.h>
#include <imgui/imgui.h>

ImVec2 ImVec(float x, float y);
ImVec2 ImVec(const DirectX::XMFLOAT2&);
ImVec2 ImVec(const DirectX::XMVECTOR&);

DirectX::XMFLOAT2 XMFLOAT(float x, float y);
DirectX::XMFLOAT2 XMFLOAT(const ImVec2&);

DirectX::XMFLOAT2 FLOAT2(const DirectX::XMVECTOR&);
DirectX::XMFLOAT3 FLOAT3(const DirectX::XMVECTOR&);
DirectX::XMFLOAT4 FLOAT4(const DirectX::XMVECTOR&);