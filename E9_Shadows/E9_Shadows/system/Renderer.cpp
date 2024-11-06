#include "Renderer.h"
#include <string>

// Configures and initilises a DirectX renderer.
// Including render states for wireframe, alpha blending and orthographics rendering.
Renderer::Renderer(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
{
	// Store rendering control variables.
	vsync_enabled = vsync;
	alphaBlendState = false;

	wnd = &hwnd;
	isFullscreen = fullscreen;
	screenheight = screenHeight;
	screenwidth = screenWidth;

	// Configure and create DirectX 11 renderer
	// include z buffer for 2D rendering and alpha blend state.
	createDevice();
	createSwapchain();

	pm_backBuffer = std::unique_ptr<BackBufferTarget>(new BackBufferTarget(DeviceInfo(), pm_swapChain.get(), screenWidth, screenHeight));

	createBlendState();
}

void Renderer::rebuild(int newWidth, int newHeight) {
	screenwidth = newWidth;
	screenheight = newHeight;

	pm_deviceContext->OMSetRenderTargets(0, 0, 0);
	pm_backBuffer = nullptr;

	HRESULT hr;
	hr = pm_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	pm_backBuffer = std::unique_ptr<BackBufferTarget>(new BackBufferTarget(DeviceInfo(), pm_swapChain.get(), newWidth, newHeight));
}

// Create a Direct3D11 rendering pm_device. Chooses the best gfx card available.
void Renderer::createDevice()
{

	// Set the feature level to DirectX 11. Other feature levels 11.1 and 11.2 require Win8+ to support it.
	//featureLevel = D3D_FEATURE_LEVEL_11_1;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	UINT creationFlags = NULL;
	//#if defined(_DEBUG)
	//	creationFlags = D3D11_CREATE_DEVICE_DEBUG;
	//#endif

		// Create a list of every GFX adapter available
		//IDXGIFactory1* pm_factory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(pm_factory.ptr_to_ptr()));

	UINT i = 0;
	IDXGIAdapter1* pAdapter;
	std::vector <IDXGIAdapter1*> vAdapters;
	while (pm_factory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pAdapter);
		++i;
	}

	// Find best GFX adapter
	SIZE_T MaxSize = 0;
	for (int ap = 0; ap < vAdapters.size(); ap++)
	{
		DXGI_ADAPTER_DESC1 desc;
		pAdapter = vAdapters[ap];
		pAdapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if ((desc.DedicatedVideoMemory > MaxSize) && (D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, creationFlags, featureLevels, 2, D3D11_SDK_VERSION, pm_device.ptr_to_ptr(), NULL, pm_deviceContext.ptr_to_ptr()) == S_OK))
		{
			//pAdapter->GetDesc1(&desc);
			//printf(L"D3D11-capable hardware found:  %s (%u MB)\n", desc.Description, desc.DedicatedVideoMemory >> 20);
			std::wstring outputS = L"D3D11 capable HW: ";
			outputS += desc.Description;
			outputS += '\n';
			OutputDebugStringW(outputS.c_str());
			MaxSize = desc.DedicatedVideoMemory;
		}
	}

}

// Create the swap chain.
void Renderer::createSwapchain()
{
	DXGI_SWAP_CHAIN_DESC pm_swapChainDesc;

	// Initialise the swap chain description.
	ZeroMemory(&pm_swapChainDesc, sizeof(pm_swapChainDesc));
	pm_swapChainDesc.BufferCount = 1;
	pm_swapChainDesc.BufferDesc.Width = screenwidth;
	pm_swapChainDesc.BufferDesc.Height = screenheight;
	//pm_swapChainDesc.BufferDesc.Width = 0;	// auto sizing
	//pm_swapChainDesc.BufferDesc.Height = 0;
	pm_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pm_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pm_swapChainDesc.OutputWindow = *wnd;
	pm_swapChainDesc.SampleDesc.Count = 1;
	pm_swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (isFullscreen)
	{
		pm_swapChainDesc.Windowed = false;
	}
	else
	{
		pm_swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	pm_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	pm_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	pm_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	pm_swapChainDesc.Flags = 0;

	// swap chain
	HRESULT h = pm_factory->CreateSwapChain(pm_device.get(), &pm_swapChainDesc, pm_swapChain.ptr_to_ptr());

}

// Creates additional raster state, in this case a aplha blending capable state.
void Renderer::createBlendState()
{
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	pm_device->CreateBlendState(&blendStateDescription, pm_alphaEnableBlendingState.ptr_to_ptr());

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	pm_device->CreateBlendState(&blendStateDescription, pm_alphaDisableBlendingState.ptr_to_ptr());
}

// Present the back buffer to the screen now rendering is complete (based on vsync switch)
void Renderer::SwapBuffers()
{
	pm_swapChain->Present(vsync_enabled ? 1 : 0, 0);
}

ID3D11Device* Renderer::Device() const { return pm_device.get(); }
ID3D11DeviceContext* Renderer::DeviceContext() const { return pm_deviceContext.get(); }

DeviceInfo Renderer::DeviceInfo() const {
	return { Device(), DeviceContext() };
}

// Sets the blending state, to enable/disable alphablending
void Renderer::setAlphaBlending(bool b)
{
	alphaBlendState = b;

	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	if (alphaBlendState)
	{
		// Turn on the alpha blending.
		pm_deviceContext->OMSetBlendState(pm_alphaEnableBlendingState.get(), blendFactor, 0xffffffff);
	}
	else
	{
		// Turn off the alpha blending.
		pm_deviceContext->OMSetBlendState(pm_alphaDisableBlendingState.get(), blendFactor, 0xffffffff);
	}
}

bool Renderer::getAlphaBlendingState()
{
	return alphaBlendState;
}

IRenderTarget& Renderer::BackBuffer() {
	return *pm_backBuffer;
}