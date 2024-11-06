#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include <system/UniqueResource.h>
#include <system/DeviceContextDependent.h>

using namespace DirectX;

class IRenderTarget : public IDeviceContextDependent
{
public:
	void Bind();
	void Clear(XMFLOAT4 clearColour = XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	XMINT2 Dimensions() const; //void SetDimensions(int, int); void SetDimensions(const XMINT2&);

	bool DepthEnabled() const; void SetDepthEnabled(bool);
	bool Wireframe() const; void SetWireframe(bool);

protected:
	IRenderTarget(const DeviceInfo& device, int width, int height);
	void Init();

	int m_width, m_height;
	D3D11_VIEWPORT m_viewport;

	bool m_wireframe = false;
	bool m_depthEnabled = true;

	unique_resource<ID3D11RenderTargetView> pm_renderTargetView;
	unique_resource<ID3D11Texture2D> pm_depthStencilBuffer;
	unique_resource<ID3D11DepthStencilState> pm_depthStencilState;
	unique_resource<ID3D11DepthStencilView> pm_depthStencilView;
	unique_resource<ID3D11RasterizerState> pm_rasterState;

	virtual void SetupRenderTargetView() = 0;
	virtual void SetupDepthStencilView();
	virtual void SetupRasterState();

	void SetupDepthStencilState();
};

class RenderTarget final : public IRenderTarget
{
public:
	RenderTarget(const DeviceInfo& device, int width, int height);

	ID3D11ShaderResourceView* Texture() const;

protected:
	unique_resource<ID3D11Texture2D> pm_texture;
	unique_resource<ID3D11ShaderResourceView> pm_shaderResourceView;

	virtual void SetupRenderTargetView() override;
};

class BackBufferTarget final : public IRenderTarget
{
public:

protected:
	BackBufferTarget(const DeviceInfo& device, IDXGISwapChain* swapChain, int width, int height);
	IDXGISwapChain* p_swapChain;

	friend class Renderer;

	virtual void SetupRenderTargetView() override;
};