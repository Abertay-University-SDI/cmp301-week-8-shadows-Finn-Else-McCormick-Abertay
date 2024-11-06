#pragma once
// Includes
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <dxgi.h>
#include <string>
#include <memory>
//#include <winerror.h>

#include <system/RenderTarget.h>
#include <system/UniqueResource.h>
#include <system/DeviceContextDependent.h>

using namespace DirectX;

class Renderer
{
public:
	Renderer(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);

	void rebuild(int newWidth, int newHeight);

	void SwapBuffers();

	DeviceInfo DeviceInfo() const; ID3D11Device* Device() const; ID3D11DeviceContext* DeviceContext() const;

	IRenderTarget& BackBuffer();

	void setAlphaBlending(bool b);	///< Sets the alpha blending state on/off for transparent rendering
	bool getAlphaBlendingState();	///< Returns alphab blending state, if on/off

private:
	void createDevice();
	void createSwapchain();
	void createBlendState();

protected:
	bool vsync_enabled;
	bool isWirefameEnabled;

	bool alphaBlendState;	///< Variable tracks alpha blending state

	bool isFullscreen;
	HWND* wnd;
	int screenheight;
	int screenwidth;

	unique_resource<IDXGIFactory1> pm_factory;
	unique_resource<IDXGISwapChain> pm_swapChain;
	unique_resource<ID3D11Device> pm_device;
	unique_resource<ID3D11DeviceContext> pm_deviceContext;

	unique_resource<ID3D11BlendState> pm_alphaEnableBlendingState, pm_alphaDisableBlendingState;

	std::unique_ptr<BackBufferTarget> pm_backBuffer;
};