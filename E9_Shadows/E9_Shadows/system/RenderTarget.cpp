// render texture
// alternative render target
#include "RenderTarget.h"

// Initialise texture object based on provided dimensions. Usually to match window.
IRenderTarget::IRenderTarget(const DeviceInfo& deviceInfo, int width, int height) : IDeviceContextDependent(deviceInfo)
{
	m_width = width;
	m_height = height;

	// Setup the viewport for rendering.
	m_viewport.Width = (float)m_width;
	m_viewport.Height = (float)m_height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
}

void IRenderTarget::Init() {
	SetupRenderTargetView();
	SetupDepthStencilView();
	SetupDepthStencilState();
	SetupRasterState();
}

void IRenderTarget::SetupDepthStencilView() {
	HRESULT result;
	// Set up the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc; ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = m_width;
	depthBufferDesc.Height = m_height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = p_device->CreateTexture2D(&depthBufferDesc, NULL, pm_depthStencilBuffer.ptr_to_ptr());

	// Set up the depth stencil view description.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = p_device->CreateDepthStencilView(pm_depthStencilBuffer.get(), &depthStencilViewDesc, pm_depthStencilView.ptr_to_ptr());
}

void IRenderTarget::SetupDepthStencilState() {
	HRESULT result;
	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc; ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = m_depthEnabled;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	//pm_depthStencilState.reset();
	result = p_device->CreateDepthStencilState(&depthStencilDesc, pm_depthStencilState.ptr_to_ptr());
	p_deviceContext->OMSetDepthStencilState(pm_depthStencilState.get(), 1);
}

void IRenderTarget::SetupRasterState() {
	D3D11_RASTERIZER_DESC rasterDesc{};
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = (m_wireframe) ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	p_device->CreateRasterizerState(&rasterDesc, pm_rasterState.ptr_to_ptr());
	p_deviceContext->RSSetState(pm_rasterState.get());
}

// Set this RenderTarget as the current render target.
// All rendering is now store here, rather than the back buffer.
void IRenderTarget::Bind()
{
	p_deviceContext->OMSetRenderTargets(1, pm_renderTargetView.ptr_to_ptr(), pm_depthStencilView.get());
	p_deviceContext->RSSetViewports(1, &m_viewport);
	p_deviceContext->RSSetState(pm_rasterState.get());
}

// Clear render texture to specified colour. Similar to clearing the back buffer, ready for the next frame.
void IRenderTarget::Clear(XMFLOAT4 col)
{
	float color[4]{ col.x, col.y, col.z, col.w };

	// Clear the back buffer and depth buffer.
	p_deviceContext->ClearRenderTargetView(pm_renderTargetView.get(), color);
	p_deviceContext->ClearDepthStencilView(pm_depthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

XMINT2 IRenderTarget::Dimensions() const {
	return XMINT2(m_width, m_height);
}

/*
void IRenderTarget::SetDimensions(const XMINT2& v) { SetDimensions(v.x, v.y); }

// Cannot for the life of me get setting the dimensions live to work properly
void IRenderTarget::SetDimensions(int x, int y) {
	m_width = x; m_height = y;
	SetupRenderTargetView();
	SetupDepthStencilView();
}
*/

bool IRenderTarget::DepthEnabled() const { return m_depthEnabled; }
void IRenderTarget::SetDepthEnabled(bool v) {
	m_depthEnabled = v;
	SetupDepthStencilState();
}

bool IRenderTarget::Wireframe() const { return m_wireframe; }
void IRenderTarget::SetWireframe(bool v) {
	m_wireframe = v;
	SetupRasterState();
}





RenderTarget::RenderTarget(const DeviceInfo& device, int width, int height) : IRenderTarget(device, width, height) {
	Init();
}

void RenderTarget::SetupRenderTargetView() {
	HRESULT result;
	// Setup the render target texture description.
	D3D11_TEXTURE2D_DESC textureDesc; ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	// Create the render target texture.
	result = p_device->CreateTexture2D(&textureDesc, NULL, pm_texture.ptr_to_ptr());

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc; ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	// Create the render target view.
	result = p_device->CreateRenderTargetView(pm_texture.get(), &renderTargetViewDesc, pm_renderTargetView.ptr_to_ptr());

	// Setup the description of the shader resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc; ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	// Create the shader resource view.
	result = p_device->CreateShaderResourceView(pm_texture.get(), &shaderResourceViewDesc, pm_shaderResourceView.ptr_to_ptr());
}

ID3D11ShaderResourceView* RenderTarget::Texture() const {
	return pm_shaderResourceView.get();
}





BackBufferTarget::BackBufferTarget(const DeviceInfo& device, IDXGISwapChain* swapChain, int width, int height) : IRenderTarget(device, width, height) {
	p_swapChain = swapChain;
	Init();
}

void BackBufferTarget::SetupRenderTargetView() {
	ID3D11Texture2D* backBufferPtr = nullptr;
	// Configure back buffer
	p_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	p_device->CreateRenderTargetView(backBufferPtr, NULL, pm_renderTargetView.ptr_to_ptr());
	backBufferPtr->Release();
	backBufferPtr = 0;
}