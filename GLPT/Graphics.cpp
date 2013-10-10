#include "Library.h"
#include "Graphics.h"
#include "Drawable.h"
#include "Window.h"

Graphics* GLPT_graphics;

EResult Graphics::Initialize(WindowData wd,std::string post_process_shader) {
	ID3D10Texture2D* dx_back_buffer;
	DXGI_SWAP_CHAIN_DESC dx_swap_desc;
	D3D10_VIEWPORT dx_view;
	D3D10_TEXTURE2D_DESC dx_depth_texture_desc,dx_render_texture_desc;
	D3D10_DEPTH_STENCIL_VIEW_DESC dx_depth_stencil_desc;
	D3D10_SHADER_RESOURCE_VIEW_DESC dx_render_texture_resource_desc;
	HRESULT hr;
	EResult output;

	this->window_data=wd;

	ZeroMemory(&dx_swap_desc,sizeof dx_swap_desc);

	dx_swap_desc.BufferCount=1;
	dx_swap_desc.BufferDesc.Width=wd.width;
	dx_swap_desc.BufferDesc.Height=wd.height;
	dx_swap_desc.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	dx_swap_desc.BufferDesc.RefreshRate.Numerator=60;
	dx_swap_desc.BufferDesc.RefreshRate.Denominator=1;
	dx_swap_desc.BufferDesc.Scaling=DXGI_MODE_SCALING_UNSPECIFIED;
	dx_swap_desc.BufferDesc.ScanlineOrdering=DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dx_swap_desc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dx_swap_desc.Flags=0;
	dx_swap_desc.OutputWindow=GLPT_window->WindowHandle();
	dx_swap_desc.SampleDesc.Count=1;
	dx_swap_desc.SampleDesc.Quality=0;
	dx_swap_desc.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
	dx_swap_desc.Windowed=!wd.fullscreen;

	// Initialize rendering devices and on-screen targets.

	hr=D3D10CreateDeviceAndSwapChain(NULL,D3D10_DRIVER_TYPE_HARDWARE,NULL,D3D10_CREATE_DEVICE_DEBUG,D3D10_SDK_VERSION,&dx_swap_desc,&dx_swap_chain,&dx_device);

	if (FAILED(hr)) {
		output.Error("Failed to create D3D10 device.");
		return output;
	}

	hr=dx_swap_chain->GetBuffer(0,__uuidof(ID3D10Texture2D),(void**) &dx_back_buffer);

	if (FAILED(hr)) {
		output.Error("Failed to index back buffer.");
		return output;
	}

	hr=dx_device->CreateRenderTargetView(dx_back_buffer,NULL,&dx_screen_target);

	if (FAILED(hr)) {
		output.Error("Failed to create screen render target.");
		return output;
	}

	// Create the off-screen textures and rendering targets.

	ZeroMemory(&dx_render_texture_desc,sizeof dx_render_texture_desc);

	dx_render_texture_desc.ArraySize=1;
	dx_render_texture_desc.BindFlags=D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	dx_render_texture_desc.CPUAccessFlags=0;
	dx_render_texture_desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	dx_render_texture_desc.Width=wd.width;
	dx_render_texture_desc.Height=wd.height;
	dx_render_texture_desc.MiscFlags=0;
	dx_render_texture_desc.MipLevels=1;
	dx_render_texture_desc.Usage=D3D10_USAGE_DEFAULT;
	dx_render_texture_desc.SampleDesc.Count=1;
	dx_render_texture_desc.SampleDesc.Quality=0;

	hr=dx_device->CreateTexture2D(&dx_render_texture_desc,NULL,&dx_render_texture);

	if (FAILED(hr)) {
		output.Error("Failed to create back rendering texture.");
		return output;
	}

	// Render texture created, create a RenderTarget to follow it.

	hr=dx_device->CreateRenderTargetView(dx_render_texture,NULL,&dx_texture_target);

	if (FAILED(hr)) {
		output.Error("Failed to create render texture target.");
		return output;
	}

	// Create the depth-stencil buffer, and then set the render target.

	ZeroMemory(&dx_depth_texture_desc,sizeof dx_depth_texture_desc);

	dx_depth_texture_desc=dx_render_texture_desc; // Reuse a few values.
	dx_depth_texture_desc.BindFlags=D3D10_BIND_DEPTH_STENCIL;
	dx_depth_texture_desc.Format=DXGI_FORMAT_D32_FLOAT;

	hr=dx_device->CreateTexture2D(&dx_depth_texture_desc,NULL,&dx_depth_texture);

	if (FAILED(hr)) {
		output.Error("Failed to create depth texture.");
		return output;
	}

	ZeroMemory(&dx_depth_stencil_desc,sizeof dx_depth_stencil_desc);

	dx_depth_stencil_desc.Format=dx_depth_texture_desc.Format;
	dx_depth_stencil_desc.Texture2D.MipSlice=0;
	dx_depth_stencil_desc.ViewDimension=D3D10_DSV_DIMENSION_TEXTURE2D;

	hr=dx_device->CreateDepthStencilView(dx_depth_texture,&dx_depth_stencil_desc,&dx_depth_stencil);

	if (FAILED(hr)) {
		output.Error("Failed to create depth-stenciling buffer.");
		return output;
	}

	dx_device->OMSetRenderTargets(1,&dx_texture_target,dx_depth_stencil);
	
	// Also create an SRV for the render texture.

	ZeroMemory(&dx_render_texture_resource_desc,sizeof dx_render_texture_resource_desc);

	dx_render_texture_resource_desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	dx_render_texture_resource_desc.Texture2D.MipLevels=1;
	dx_render_texture_resource_desc.ViewDimension=D3D10_SRV_DIMENSION_TEXTURE2D;

	hr=dx_device->CreateShaderResourceView(dx_render_texture,&dx_render_texture_resource_desc,&dx_render_texture_resource);

	if (FAILED(hr)) {
		output.Error("Failed to create render texture SRV.");
		return output;
	}

	shader_post_process=new Shader;
	if (shader_post_process->Initialize(post_process_shader+".hlsl",BasicVertex::layout,BasicVertex::layout_size).Error()) {
		GLPT_logger.Print("[GLPT_graphics] Failed to load post processing shader.");
	} else {
		GLPT_logger.Print("[GLPT_graphics] Loaded post processing shader. [" + post_process_shader + std::string("]"));
	}

	BasicVertex vertices[6];

	BasicVertex::make_rectangle(vertices,1.0f,1.0f);

	render_drawable.Load(vertices,6,shader_post_process);
	render_drawable.Texturize(dx_render_texture_resource);

	GLPT_logger.Print("[GLPT_graphics] Loaded render drawable object.");

	dx_view.Width=wd.width;
	dx_view.Height=wd.height;
	dx_view.MaxDepth=1.0f;
	dx_view.MinDepth=0.0f;
	dx_view.TopLeftX=0;
	dx_view.TopLeftY=0;

	dx_device->RSSetViewports(1,&dx_view);

	return output;

}

void Graphics::Clear(void) {
	float cl_color[4]={0.0f,0.2f,0.27f,0.0f};

	dx_device->ClearRenderTargetView(dx_texture_target,cl_color);
	dx_device->ClearDepthStencilView(dx_depth_stencil,D3D10_CLEAR_DEPTH,1.0f,0);
}

void Graphics::Present(void) {
	float cl_color[4]={0.5f,0.0f,0.1f,0.0f};

	dx_device->OMSetRenderTargets(1,&dx_screen_target,dx_depth_stencil);
	dx_device->ClearRenderTargetView(dx_screen_target,cl_color);

	// Now render the texture onto the screen through the post-processing shader.

	// Also switch back to texture rendering.

	GLPT_camera->Set2D(true);
	render_drawable.Draw();
	GLPT_camera->Set2D(false);

	// Drawable bound the render texture, now unbind it.

	shader_post_process->Texture(NULL);
	shader_post_process->Bind();

	dx_device->OMSetRenderTargets(1,&dx_texture_target,dx_depth_stencil);

	dx_swap_chain->Present((window_data.vertical_sync) ? 1 : 0,0);
}



Graphics::~Graphics(void) {

	dx_screen_target->Release();
	dx_texture_target->Release();
	dx_render_texture->Release();
	dx_render_texture_resource->Release();
	dx_depth_stencil->Release();
	dx_depth_texture->Release();
	dx_swap_chain->SetFullscreenState(false,NULL);
	dx_swap_chain->Release();
	dx_device->Release();

}

